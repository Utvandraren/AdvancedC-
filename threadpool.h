
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <cmath>
#include <algorithm>
#include <future>


class ThreadPool {
public:
    std::thread* _threads;
    std::vector<std::function<void()>*> _queue;
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
            function = *_queue.front();
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
    std::future<F> enqueue(F&& f, Args&&... args) {

        //if there is not enough threads ready then put the function in the function queue
        //send the function to a already ready thread --> how to keep the thread idle? just use the waitfunction?

        auto task = std::make_shared
            < std::packaged_task<F> >           //What is the type of the providided work function, F?
            (
                std::bind(std::forward<F>(f),
                    std::forward<Args>(args)...)
                );
        std::future<F> future = task.get_future();  // get a future
        std::unique_lock<std::mutex> lock(mut);

        _queue.push_back([task]() { (*task)(); });
        cv.notify_one();
        return future;
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


