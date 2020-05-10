/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/21/2020
- End Header ----------------------------*/
#pragma once

// Reference: https://raw.githubusercontent.com/progschj/ThreadPool/master/ThreadPool.h
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "engine/EngineCore.h"

namespace gswy
{
    class ThreadPool {
    public:
        NONCOPYABLE(ThreadPool);
        // the constructor just launches some amount of workers
        ThreadPool();
        // the destructor joins all threads
        ~ThreadPool();
        // add new work item to the pool
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args)
            ->std::future<typename std::result_of<F(Args...)>::type>;
    private:
        // need to keep track of threads so we can join them
        std::vector< std::thread > workers;
        // the task queue
        std::queue< std::function<void()> > tasks;

        // synchronization
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };


    // add new work item to the pool

    template<class F, class ...Args>
    inline auto ThreadPool::enqueue(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args ...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

}