#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "sim_config.hpp"
#include "system_executor.hpp"
#include "executor_factory.hpp"
#include "model_wait_and_go.hpp"

namespace evsim {

class SchedulerOptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_factory = std::make_unique<CExecutorFactory>();
    }

    double run_benchmark(bool optimized) {
        SimConfig config{};
        config.engine_name = optimized ? "optimized" : "legacy";
        config.execution_mode = BLOCKING;
        config.simulation_mode = VIRTUAL;
        config.time_resolution = 1;
        config.use_optimized_scheduler = optimized;
        config.ef = m_factory.get();

        std::unique_ptr<CSystemExecutor> executor(CSystemExecutor::create_system_executor(config));

        constexpr int kModelCount = 2000;
        for (int i = 0; i < kModelCount; ++i) {
            auto name = std::string("model_") + std::to_string(i);
            CWaitGEN* model = new CWaitGEN(name);
            executor->register_entity(model, static_cast<Time>(i), Infinity);
        }

        auto start = std::chrono::steady_clock::now();
        executor->simulate(kModelCount);
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end - start;
        return elapsed.count();
    }

    std::unique_ptr<CExecutorFactory> m_factory;
};

TEST_F(SchedulerOptionTest, optimizedSchedulerImprovesRuntime) {
    const double legacy_ms = run_benchmark(false);
    const double optimized_ms = run_benchmark(true);

    RecordProperty("legacy_ms", legacy_ms);
    RecordProperty("optimized_ms", optimized_ms);

    ASSERT_GT(legacy_ms, 0.0);
    ASSERT_GT(optimized_ms, 0.0);
    EXPECT_LE(optimized_ms, legacy_ms * 1.05);
}

} // namespace evsim
