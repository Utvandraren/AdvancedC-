
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <cmath>
#include <algorithm>
#include <future>


class ThreadPool {
public:
	std::vector<std::thread> _threads;
	std::vector<std::function<void()>> _queue;
	std::condition_variable cv;
	std::mutex mut;
	bool isShuttingDown = false;

	void ThreadFunction() {
		std::function<void()> function;
		while (true) {
			{
				std::unique_lock<std::mutex> lock(mut);
				cv.wait(lock, [&] { return isShuttingDown || !_queue.empty(); });
				if (isShuttingDown)
					return;
				function = _queue.front();
				_queue.erase(_queue.begin());
			}
			function();
		}
	}

	~ThreadPool() {
		isShuttingDown = true;
		cv.notify_all();
		for (size_t i = 0; i < _threads.size(); i++)
		{
			_threads[i].join();
		}
	}

	//used by main (you are free to change here and in main)
	ThreadPool(size_t threads) {
		for (size_t i = 0; i < threads; i++)
		{
			_threads.push_back(std::thread([&]() { (ThreadFunction)(); }));
		}
	}

	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args) {
		auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::unique_lock<std::mutex> lock(mut);
		std::function<void()> wrappedTask = [task]() { (*task)(); };

		_queue.push_back(wrappedTask);
		cv.notify_one();
		return task->get_future(); ;
	}
};
#endif


