
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <cmath>
#include <algorithm>
#include <future>


class ThreadPool {
public:
    std::thread* _threads;
    std::vector<std::function<void()>> _queue;
    std::condition_variable cv;
    std::mutex mut;
    int _activeThreads = 0;
    size_t _maxThreads;
    bool isShuttingDown = false;

    void ThreadFunction() {
        std::function<void()> function;
        while (true) {
            std::unique_lock<std::mutex> lock(mut);
            cv.wait(lock);
            if (isShuttingDown)
                return;
            function = _queue.front();
            _queue.erase(_queue.begin());
            function();    //Has to save the specific parameter you are calling too
        }
    }

    ~ThreadPool() {
        //Do more here?
        isShuttingDown = true;
        cv.notify_all();
        for (size_t i = 0; i < _maxThreads; i++)
        {
            _threads[i].join();
        }      
    }

    //used by main (you are free to change here and in main)
    ThreadPool(size_t threads) {
        _maxThreads = threads;
        _threads = new std::thread[threads];
        for (size_t i = 0; i < threads; i++)
        {
            _threads[i] = std::thread([]() {while (true) {}});
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) {

        //if there is not enough threads ready then put the function in the function queue
        //using return_type = std::invoke_result_t(F, Args...);
        auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(std::bind(std::forward<F>(f),std::forward<Args>(args)...));


        std::function<void()> wrappedTask = [task]() { (*task)(); };
        _queue.push_back(wrappedTask);
        cv.notify_one();
        return task->get_future(); ;
    }

    //template<class F, class... Args>
    //auto packaged_task(F&& f, Args&&... args) {
    //    //auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    //    auto task = std::make_shared
    //        <std::packaged_task<return_type()>>
    //        (
    //            std::bind(std::forward<F>(f),
    //                std::forward<Args>(args)...)
    //            );
    //    return task;
    //}
};
#endif


