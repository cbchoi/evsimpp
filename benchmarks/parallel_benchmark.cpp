#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "sim_config.hpp"
#include "system_executor.hpp"
#include "executor_factory.hpp"
#include "model_wait_and_go.hpp"
#include "model_buffer.hpp"
#include "model_skeleton_coupled.hpp"

using namespace evsim;

namespace {

CSkeletonCoupledModel* make_vertical_chain(int depth)
{
    if (depth <= 0) {
        return nullptr;
    }

    if (depth == 1) {
        auto* buffer = new CBuffer("buffer_leaf");
        auto* coupled = new CSkeletonCoupledModel("level_1");
        Port& in_port = coupled->create_input_port("sc_input");
        Port& out_port = coupled->create_output_port("sc_output");

        coupled->insert_model(buffer);
        coupled->insert_coupling(coupled, in_port, buffer, buffer->input);
        coupled->insert_coupling(buffer, buffer->output, coupled, out_port);
        return coupled;
    }

    auto* child = make_vertical_chain(depth - 1);
    auto* coupled = new CSkeletonCoupledModel("level_" + std::to_string(depth));
    Port& in_port = coupled->create_input_port("sc_input");
    Port& out_port = coupled->create_output_port("sc_output");

    coupled->insert_model(child);
    coupled->insert_coupling(coupled, in_port, child, *child->find_dyn_in_port("sc_input"));
    coupled->insert_coupling(child, *child->find_dyn_out_port("sc_output"), coupled, out_port);
    return coupled;
}

struct BenchResult {
    ParallelMode mode;
    std::string scenario;
    double millis;
    std::size_t outputs;
    int generator_events;
    int buffer_events;
};

class CHeavyBuffer : public CBuffer
{
public:
    CHeavyBuffer(std::string name, int work_cost)
        : CBuffer(std::move(name)), m_work_cost(work_cost), m_accumulator(0)
    {
    }

    void internal_transition() override
    {
        volatile int local = m_accumulator;
        for (int i = 0; i < m_work_cost; ++i)
        {
            local += (i & 1) ? 1 : -1;
        }
        m_accumulator = local;
        CBuffer::internal_transition();
    }

private:
    int m_work_cost;
    int m_accumulator;
};

BenchResult run_horizontal_bench(ParallelMode mode, int steps, int fanout)
{
    CExecutorFactory factory;
    SimConfig config{};
    config.engine_name = "bench_horizontal";
    config.time_resolution = 1;
    config.execution_mode = BLOCKING;
    config.simulation_mode = VIRTUAL;
    config.use_optimized_scheduler = true;
    config.parallel_mode = mode;
    config.parallel_workers = 8;
    config.ef = &factory;

    std::unique_ptr<CSystemExecutor> executor(CSystemExecutor::create_system_executor(config));

    Port& input = executor->create_input_port("engine_input");
    Port& output = executor->create_output_port("engine_output");
    executor->register_input_port(input);
    executor->register_output_port(output);

    auto* generator = new CWaitGEN("gen");
    executor->register_entity(generator, 0, Infinity);
    executor->insert_coupling(executor.get(), input, generator, generator->input);

    std::vector<CBuffer*> buffers;
    for (int i = 0; i < fanout; ++i) {
        auto* buffer = new CBuffer("buffer_" + std::to_string(i));
        executor->register_entity(buffer, 0, Infinity);
        executor->insert_coupling(generator, generator->output, buffer, buffer->input);
        executor->insert_coupling(buffer, buffer->output, executor.get(), output);
        buffers.push_back(buffer);
    }

    Message msg = executor->create_message(input);
    executor->insert_external_event(msg);

    auto start = std::chrono::steady_clock::now();
    executor->simulate(steps);
    auto stop = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(stop - start).count();
    std::size_t output_count = executor->get_external_output_deliverer().get_contents().size();
    int gen_events = generator->elem_count;
    int buffer_events = 0;
    for (auto* buffer : buffers) {
        buffer_events += buffer->elem_count;
    }

    return BenchResult{mode, "horizontal", elapsed, output_count, gen_events, buffer_events};
}

BenchResult run_vertical_bench(ParallelMode mode, int steps, int depth)
{
    CExecutorFactory factory;
    SimConfig config{};
    config.engine_name = "bench_vertical";
    config.time_resolution = 1;
    config.execution_mode = BLOCKING;
    config.simulation_mode = VIRTUAL;
    config.use_optimized_scheduler = true;
    config.parallel_mode = mode;
    config.parallel_workers = 8;
    config.ef = &factory;

    std::unique_ptr<CSystemExecutor> executor(CSystemExecutor::create_system_executor(config));

    Port& input = executor->create_input_port("engine_input");
    Port& output = executor->create_output_port("engine_output");
    executor->register_input_port(input);
    executor->register_output_port(output);

    auto* generator = new CWaitGEN("gen");
    executor->register_entity(generator, 0, Infinity);
    executor->insert_coupling(executor.get(), input, generator, generator->input);

    std::unique_ptr<CSkeletonCoupledModel> chain(make_vertical_chain(depth));
    executor->register_entity(chain.get(), 0, Infinity);
    executor->insert_coupling(generator, generator->output, chain.get(), *chain->find_dyn_in_port("sc_input"));
    executor->insert_coupling(chain.get(), *chain->find_dyn_out_port("sc_output"), executor.get(), output);

    Message msg = executor->create_message(input);
    executor->insert_external_event(msg);

    auto start = std::chrono::steady_clock::now();
    executor->simulate(steps);
    auto stop = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(stop - start).count();
    std::size_t output_count = executor->get_external_output_deliverer().get_contents().size();
    int gen_events = generator->elem_count;
    int buffer_events = 0;

    // prevent the executor from deleting the chain twice; ownership transferred
    chain.release();

    return BenchResult{mode, "vertical", elapsed, output_count, gen_events, buffer_events};
}

BenchResult run_heavy_internal_bench(ParallelMode mode, int steps, int fanout, int work_cost)
{
    CExecutorFactory factory;
    SimConfig config{};
    config.engine_name = "bench_heavy";
    config.time_resolution = 1;
    config.execution_mode = BLOCKING;
    config.simulation_mode = VIRTUAL;
    config.use_optimized_scheduler = true;
    config.parallel_mode = mode;
    config.parallel_workers = 8;
    config.ef = &factory;

    std::unique_ptr<CSystemExecutor> executor(CSystemExecutor::create_system_executor(config));

    Port& input = executor->create_input_port("engine_input");
    Port& output = executor->create_output_port("engine_output");
    executor->register_input_port(input);
    executor->register_output_port(output);

    auto* generator = new CWaitGEN("gen");
    executor->register_entity(generator, 0, Infinity);
    executor->insert_coupling(executor.get(), input, generator, generator->input);

    std::vector<CHeavyBuffer*> buffers;
    buffers.reserve(fanout);

    for (int i = 0; i < fanout; ++i)
    {
        auto* buffer = new CHeavyBuffer("heavy_" + std::to_string(i), work_cost);
        executor->register_entity(buffer, 0, Infinity);
        executor->insert_coupling(generator, generator->output, buffer, buffer->input);
        executor->insert_coupling(buffer, buffer->output, executor.get(), output);
        buffers.push_back(buffer);
    }

    Message msg = executor->create_message(input);
    executor->insert_external_event(msg);

    auto start = std::chrono::steady_clock::now();
    executor->simulate(steps);
    auto stop = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(stop - start).count();
    std::size_t output_count = executor->get_external_output_deliverer().get_contents().size();
    int gen_events = generator->elem_count;
    int buffer_events = 0;
    for (auto* buffer : buffers)
    {
        buffer_events += buffer->elem_count;
    }

    return BenchResult{mode, "heavy_internal", elapsed, output_count, gen_events, buffer_events};
}

void print_result(const BenchResult& result)
{
    const char* mode = result.mode == ParallelMode::Serial ? "Serial"
                         : (result.mode == ParallelMode::ParallelEventBatch ? "ParallelBatch" : "ParallelRouting");
    std::cout << "scenario=" << result.scenario
              << " mode=" << mode
              << " time_ms=" << result.millis
              << " outputs=" << result.outputs
              << " generator_events=" << result.generator_events
              << " buffer_events=" << result.buffer_events
              << '\n';
}

} // namespace

int main()
{
    const int horizontal_steps = 200;
    const int horizontal_fanout = 400;
    const int vertical_steps = 200;
    const int vertical_depth = 120;

    std::vector<BenchResult> results;
    results.push_back(run_horizontal_bench(ParallelMode::Serial, horizontal_steps, horizontal_fanout));
    results.push_back(run_horizontal_bench(ParallelMode::ParallelEventBatch, horizontal_steps, horizontal_fanout));
    results.push_back(run_vertical_bench(ParallelMode::Serial, vertical_steps, vertical_depth));
    results.push_back(run_vertical_bench(ParallelMode::ParallelEventBatch, vertical_steps, vertical_depth));
    results.push_back(run_heavy_internal_bench(ParallelMode::Serial, 50, 200, 20000));
    results.push_back(run_heavy_internal_bench(ParallelMode::ParallelEventBatch, 50, 200, 20000));

    for (const auto& result : results) {
        print_result(result);
    }

    return 0;
}
