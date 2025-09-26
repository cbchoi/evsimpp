#pragma once

#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "sim_config.hpp"
#include "system_executor.hpp"
#include "executor_factory.hpp"
#include "model_wait_and_go.hpp"
#include "model_buffer.hpp"

namespace evsim {

class ParallelModeTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = std::make_unique<CExecutorFactory>();
    }

    std::unique_ptr<CSystemExecutor> make_executor(ParallelMode mode) {
        SimConfig config{};
        config.engine_name = "parallel_mode_test";
        config.time_resolution = 1;
        config.execution_mode = BLOCKING;
        config.simulation_mode = VIRTUAL;
        config.use_optimized_scheduler = true;
        config.parallel_mode = mode;
        config.ef = factory.get();
        return std::unique_ptr<CSystemExecutor>(CSystemExecutor::create_system_executor(config));
    }

    std::unique_ptr<CExecutorFactory> factory;
};

TEST_F(ParallelModeTest, SerialModeMatchesBehaviour)
{
    auto executor = make_executor(ParallelMode::Serial);

    Port& input = executor->create_input_port("engine_input");
    executor->register_input_port(input);

    auto* generator = new CWaitGEN("serial_gen");
    executor->register_entity(generator, 0, Infinity);
    executor->insert_coupling(executor.get(), input, generator, generator->input);

    Message msg = executor->create_message(input);
    executor->insert_external_event(msg);

    executor->simulate(10);
    EXPECT_GE(generator->elem_count, 10);
}

TEST_F(ParallelModeTest, ParallelBatchModeProducesOutput)
{
    auto executor = make_executor(ParallelMode::ParallelEventBatch);

    Port& input = executor->create_input_port("engine_input");
    executor->register_input_port(input);

    auto* generator = new CWaitGEN("parallel_gen");
    executor->register_entity(generator, 0, Infinity);
    executor->insert_coupling(executor.get(), input, generator, generator->input);

    Message msg = executor->create_message(input);
    executor->insert_external_event(msg);

    executor->simulate(10);
    EXPECT_GE(generator->elem_count, 10);
}

TEST_F(ParallelModeTest, ParallelBatchMatchesSerialRouting)
{
    constexpr int steps = 20;
    constexpr int fanout = 8;

    struct ScenarioResult {
        int generator_events;
        int buffer_events;
        std::size_t output_events;
    };

    auto runScenario = [&](std::unique_ptr<CSystemExecutor>& executor) {
        ScenarioResult result{};

        Port& input = executor->create_input_port("engine_input");
        executor->register_input_port(input);

        Port& output = executor->create_output_port("engine_output");
        executor->register_output_port(output);

        auto* generator = new CWaitGEN("routing_gen");
        executor->register_entity(generator, 0, Infinity);
        executor->insert_coupling(executor.get(), input, generator, generator->input);

        std::vector<CBuffer*> buffers;
        buffers.reserve(fanout);

        for (int i = 0; i < fanout; ++i) {
            auto* buffer = new CBuffer("buffer_" + std::to_string(i));
            executor->register_entity(buffer, 0, Infinity);
            executor->insert_coupling(generator, generator->output, buffer, buffer->input);
            executor->insert_coupling(buffer, buffer->output, executor.get(), output);
            buffers.push_back(buffer);
        }

        Message msg = executor->create_message(input);
        executor->insert_external_event(msg);

        executor->simulate(steps);

        result.generator_events = generator->elem_count;
        for (auto* buffer : buffers) {
            result.buffer_events += buffer->elem_count;
        }
        result.output_events = executor->get_external_output_deliverer().get_contents().size();

        return result;
    };

    auto serial_executor = make_executor(ParallelMode::Serial);
    auto serial_result = runScenario(serial_executor);

    auto parallel_executor = make_executor(ParallelMode::ParallelEventBatch);
    auto parallel_result = runScenario(parallel_executor);

    EXPECT_EQ(serial_result.generator_events, steps);
    EXPECT_EQ(serial_result.generator_events, parallel_result.generator_events);
    EXPECT_EQ(serial_result.buffer_events, parallel_result.buffer_events);
    EXPECT_EQ(serial_result.output_events, parallel_result.output_events);

    EXPECT_EQ(serial_result.output_events, serial_result.buffer_events);
    EXPECT_EQ(parallel_result.output_events, parallel_result.buffer_events);
}

} // namespace evsim
