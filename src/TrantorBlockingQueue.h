#pragma once
#include <queue>
#include "TrantorCondition.h"

namespace trantor
{
	template <class T>
	class TrantorBlockingQueue
	{
	public:
		explicit TrantorBlockingQueue(uint32_t max_queue_size) 
		: max_queue_size_(max_queue_size),
		mtx_(),
		high_water_level_tc_(mtx_),
		empty_tc_(mtx_){}
		T take();
		void put(const T& element);

	private:
		std::queue<T> queue_;
		uint32_t max_queue_size_;
		std::mutex mtx_;
		TrantorCondition high_water_level_tc_;
		TrantorCondition empty_tc_;
	};

	template <class T>
	void TrantorBlockingQueue<T>::put(const T& element)
	{
		std::lock_guard<mutex> lck(mtx_);
		high_water_level_tc_.wait(lck, [=]() {return queue_.size() < max_queue_size_});
		queue_.push(element);
		empty_tc_.notifyOne();
	}

	template <class T>
	T TrantorBlockingQueue<T>::take()
	{
		std::lock_guard<mutex> lck(mtx_);
		while (queue_.empty())
		{
			empty_tc_.wait();
		}
		T& ret = queue_.front();
		queue_.pop();
		high_water_level_tc_.notify()
		return ret;
	}
}

