
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <cmath>
#include <algorithm>
#include <future>


class ThreadPool {
public:
    std::thread* _threads;
    std::function<class F>* _queue;
    std::condition_variable cv;
    std::mutex mut;


    //used by main (you are free to change here and in main)
    ThreadPool(size_t threads) {
        _threads = new std::thread[threads];
        _queue = new std::function<class F>[threads];
        
    }



    template<class F, class... Args>
    std::future<F> enqueue(F&& f, Args&&... args) {
        //return packaged_task(F && f, Args&&... args);
          // future from a packaged_task
        std::packaged_task<F> task = packaged_task(F && f, Args&&... args);  // wrap the function
        std::future<F> f1 = task.get_future();  // get a future
        //add mutex or cond var here?
        std::thread t(std::move(task)); // launch on a thread
    }

    /* todo: your implementation and more methods and variables*/
    template<class F, class... Args>
    auto packaged_task(F&& f, Args&&... args) {
        //auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto task = std::make_shared
            <std::packaged_task<return_type()>>
            (
                std::bind(std::forward<F>(f),
                    std::forward<Args>(args)...)
                );
        return task;
    }
};

#endif


