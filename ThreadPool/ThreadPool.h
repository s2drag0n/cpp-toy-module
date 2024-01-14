#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool {
  public:
    ThreadPool(size_t thread_num);

    ~ThreadPool();

    /* 向任务队列中加入新的任务 */
    template <class F, class... Arg>
    auto enqueue(F &&f, Arg &&...arg)
        -> std::future<typename std::result_of<F(Arg...)>::type>;

  private:
    /* 工作线程数组 */
    std::vector<std::thread> workers;

    /* 任务队列 */
    std::queue<std::function<void()>> tasks;

    /* 队列的锁 */
    std::mutex mutex;

    /* 条件变量 */
    std::condition_variable condition;

    /* 线程池是否停止工作 */
    bool stop;
};

inline ThreadPool::ThreadPool(size_t thread_num) : stop(false) {
    for (int i = 0; i < thread_num; ++i) {
        workers.emplace_back([this]() {
            std::function<void()> task;

            while (true) {
                {
                    std::unique_lock<std::mutex> lock(mutex);

                    /* 只有在线程池停止或者队列不为空时，才重新获得锁结束等待状态
                     */
                    this->condition.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty();
                    });

                    /* 如果线程池停止了，并且没有任务可以做，就退出 */
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    task = std::move(this->tasks.front());

                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }

    condition.notify_all();

    for (auto &worker : workers) {
        worker.join();
    }
}

template <class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type> {

    using return_type = typename std::result_of<F(Args...)>::type;

    /* packaged_task封装一个可调用对象，并且可以从其中获取到future，
     * 所以这里将传入的任务封装到packaged_task中
     */
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(mutex);
        if (stop) {
            throw std::runtime_error("该线程池已经关闭，无法插入新的任务");
        }
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();

    return res;
}
