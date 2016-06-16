#ifndef TRANTOR_THREAD_POOL_H_
#define TRANTOR_THREAD_POOL_H_

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <memory>
#include "TrantorNonCopyable.h"

namespace trantor
{
	class TrantorFixedThreadPool :TrantorNonCopyable
	{
	public:
		explicit TrantorFixedThreadPool(const int thread_num);
		~TrantorFixedThreadPool();
		void pushTask(std::function<void()> func);
		void waitUntilFinished();
		bool isFree();
		uint64_t getTaskNumInQueue()
		{
			std::unique_lock<std::mutex> lock(mtx_);
			return task_queue_.size();
		}
	private:
		std::queue<std::function<void()> > task_queue_;
		std::vector<std::shared_ptr<std::thread> > thread_queue_;
		std::mutex mtx_;
		std::condition_variable not_empty_cv_;
		std::condition_variable empty_cv_;
		std::atomic<bool> wait_empty_;
		std::atomic<bool> pool_alive_;
		long left_task_num_;

		void threadFunc();
	};

	class TrantorSingleThread : TrantorNonCopyable
	{
	public:
		explicit TrantorSingleThread():thread_pool_(1) {}
		void pushTask(std::function<void()> func)
		{
			thread_pool_.pushTask(func);
		}
		void waitUntilFinished()
		{
			thread_pool_.waitUntilFinished();
		}
		bool isFree()
		{
			return thread_pool_.isFree();
		}
		uint64_t getTaskNumInQueue()
		{
			return thread_pool_.getTaskNumInQueue();
		}
	private:
		TrantorFixedThreadPool thread_pool_;
	};

	class TrantorCachedThreadPool : TrantorNonCopyable
	{
	public:
		explicit TrantorCachedThreadPool():max_thread_num_(INT16_MAX){}
		TrantorCachedThreadPool(const uint16_t max_thread_num) :max_thread_num_(max_thread_num) {}
		void pushTask(std::function<void()> func);
		uint16_t getThreadNum();
		void reset();
		void waitUntilFinished();
	private:
		std::vector<std::shared_ptr<TrantorSingleThread> > single_thread_vec_;
		std::mutex mtx_;
		uint16_t max_thread_num_;
	};
}

#endif
