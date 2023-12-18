#ifndef JOBS_HPP
#define JOBS_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

namespace job {

    class queue {
    public:
        queue(const int& size) : m_terminate(false) {
            for (int i = 0; i < size; ++i) {
                m_threads.emplace_back([this]() {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(m_tasks_mutex);
                            m_tasks_cv.wait(lock, [this]() {
                                return m_terminate || !m_tasks.empty();
                            });
                            if (m_terminate && m_tasks.empty()) {
                                return;
                            }
                            task = m_tasks.front();
                            m_tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~queue() {
            {
                std::unique_lock<std::mutex> lock(m_tasks_mutex);
                m_terminate = true;
            }
            m_tasks_cv.notify_all();
            for (auto& thread : m_threads) {
                thread.join();
            }
        }

        
        template<typename F, typename... Args>
        void enqueue(F&& f, Args&&... args) {
            {
                std::unique_lock<std::mutex> lock(m_tasks_mutex);
                m_tasks.emplace([f, args...]() {
                    f(args...);
                });
            }
            m_tasks_cv.notify_one();
        }

    private:
        std::vector<std::thread> m_threads;
        std::queue<std::function<void()>> m_tasks;
        std::mutex m_tasks_mutex;
        std::condition_variable m_tasks_cv;
        bool m_terminate;
    };

}

#endif // JOBS_HPP