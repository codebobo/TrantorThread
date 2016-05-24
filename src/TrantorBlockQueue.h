#pragma once
#include <queue>
#include <condition_variable>
#include "TrantorNonCopyable.h"

namespace trantor
{
	template <class T>
	class TrantorBlockQueue : TrantorNonCopyable
	{
	public:
		explicit TrantorBlockQueue(uint32_t max_queue_size) 
		: max_queue_size_(max_queue_size),
		mtx_(){}
		~TrantorBlockQueue();
		T take();
		void put(const T& element);
		uint32_t size() 
		{
			std::lock_guard<mutex> lck(mtx_);
			return queue_.size();
		}

	private:
		std::queue<T> queue_;
		uint32_t max_queue_size_;
		std::mutex mtx_;
		std::condition_variable high_water_level_tc_;
		std::condition_variable empty_tc_;
	};

	template <class T>
	void TrantorBlockQueue<T>::put(const T& element)
	{
		std::unique_lock<mutex> lck(mtx_);
		high_water_level_tc_.wait(lck, [=]() {return queue_.size() < max_queue_size_; });
		if (queue_.size() < max_queue_size_)
		{
			queue_.push(element);
			empty_tc_.notify_all();
		}
	}

	template <class T>
	T TrantorBlockQueue<T>::take()
	{
		std::unique_lock<mutex> lck(mtx_);
		empty_tc_.wait(lck, [=](){return !queue_.empty(); });
		if (!queue_.empty())
		{
			T& ret = queue_.front();
			queue_.pop();
			if (queue_.size() < max_queue_size_)
			{
				high_water_level_tc_.notify_all();
			}
			return ret;
		}
	}

	template <class T>
	trantor::TrantorBlockQueue<T>::~TrantorBlockQueue()
	{
		high_water_level_tc_.notify_all();
		empty_tc_.notify_all();
	}
}

