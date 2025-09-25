#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace evsim {

class ThreadPool
{
public:
    explicit ThreadPool(std::size_t worker_count) : m_stop(false), m_active_tasks(0)
    {
        if (worker_count == 0) {
            worker_count = 1;
        }

        m_workers.reserve(worker_count);
        m_worker_count = worker_count;
        for (std::size_t i = 0; i < worker_count; ++i) {
            m_workers.emplace_back([this]() { worker_loop(); });
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_cv.notify_all();

        for (auto& worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename Fn>
    void enqueue(Fn&& fn)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tasks.emplace(std::forward<Fn>(fn));
        }
        m_cv.notify_one();
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv_finished.wait(lock, [this]() { return m_tasks.empty() && m_active_tasks == 0; });
    }

    std::size_t worker_count() const noexcept
    {
        return m_worker_count;
    }

private:
    void worker_loop()
    {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this]() { return m_stop || !m_tasks.empty(); });

                if (m_stop && m_tasks.empty()) {
                    return;
                }

                task = std::move(m_tasks.front());
                m_tasks.pop();
                ++m_active_tasks;
            }

            task();

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                --m_active_tasks;
                if (m_tasks.empty() && m_active_tasks == 0) {
                    m_cv_finished.notify_all();
                }
            }
        }
    }

private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::condition_variable m_cv_finished;
    bool m_stop;
    std::size_t m_active_tasks;
    std::size_t m_worker_count{0};
};

} // namespace evsim
