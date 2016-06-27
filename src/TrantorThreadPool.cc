#include "TrantorThreadPool.h"
#include <assert.h>
#include <iostream>
using namespace std;
//static long cnt1 = 0;
//static long cnt2 = 0;
namespace trantor
{
	TrantorFixedThreadPool::TrantorFixedThreadPool(const int thread_num):pool_alive_(true), wait_empty_(false), left_task_num_(0)
	{
		assert(thread_num > 0);
		for(int i = 0; i < thread_num; i++)
		{
			std::shared_ptr<std::thread> th_ptr = make_shared<std::thread>(&TrantorFixedThreadPool::threadFunc, this);
			thread_queue_.push_back(th_ptr);
		}
	}
	TrantorFixedThreadPool::~TrantorFixedThreadPool()
	{	
		auto f = [=](std::shared_ptr<std::thread> iter)
		{
			if (iter)
			{
				iter->join();
			}
		};
		{
			std::unique_lock<std::mutex> lock(mtx_);
			pool_alive_ = false;
			not_empty_cv_.notify_all();
		}
		for_each(thread_queue_.begin(), thread_queue_.end(), f);
	}
	void TrantorFixedThreadPool::pushTask(std::function<void()> func)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		task_queue_.push(func);
		left_task_num_++;
		//cout << "task push num: " << cnt1++<<" "<< task_queue_.size() << " " << std::this_thread::get_id() <<endl;
		not_empty_cv_.notify_one(); 
	}

	void TrantorFixedThreadPool::waitUntilFinished()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		if (left_task_num_ == 0)
		{
			return;
		}
		else
		{
			wait_empty_ = true;
			empty_cv_.wait(lock, [=]() {return task_queue_.empty(); });
			wait_empty_ = false;
		}
	}
	bool TrantorFixedThreadPool::isFree()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return task_queue_.empty();
	}
	void TrantorFixedThreadPool::threadFunc()
	{
		while(pool_alive_)
		{
			std::function<void()> func_to_do;
			{
				std::unique_lock<std::mutex> lock(mtx_);
				if (pool_alive_)
				{
					if (wait_empty_ && left_task_num_ == 0)
					{
						empty_cv_.notify_one();
					}
					not_empty_cv_.wait(lock, [=]() {return (!task_queue_.empty() || !pool_alive_); });
					if (!task_queue_.empty())
					{
						func_to_do = task_queue_.front();
						task_queue_.pop();
						//cout << "task done num: " << cnt2++ << " " << std::this_thread::get_id() << endl;
					}
				}
			}
			if (func_to_do)
			{
				func_to_do();
				{
					std::unique_lock<std::mutex> lock(mtx_);
					left_task_num_--;
				}
			}
			
		}
	}
	void TrantorCachedThreadPool::pushTask(std::function<void()> func)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		if (single_thread_vec_.size() == 0)
		{
			std::shared_ptr<TrantorSingleThread> thread_ptr = std::make_shared<TrantorSingleThread>();
			assert(thread_ptr);
			thread_ptr->pushTask(func);
			single_thread_vec_.push_back(thread_ptr);
		}
		else
		{
			std::vector<std::shared_ptr<TrantorSingleThread> >::iterator iter_tag = single_thread_vec_.begin();
			for (auto iter = single_thread_vec_.begin(); iter != single_thread_vec_.end(); ++iter)
			{
				if ((*iter)->isFree())
				{
					(*iter)->pushTask(func);
					return;
				}
				if ((*iter)->getTaskNumInQueue() < (*iter_tag)->getTaskNumInQueue())
				{
					iter_tag = iter;
				}
			}
			if (single_thread_vec_.size() < max_thread_num_)
			{
				std::shared_ptr<TrantorSingleThread> thread_ptr = std::make_shared<TrantorSingleThread>();
				assert(thread_ptr);
				thread_ptr->pushTask(func);
				single_thread_vec_.push_back(thread_ptr);
			}
			else
			{
				(*iter_tag)->pushTask(func);
			}
		}
	}
	uint16_t TrantorCachedThreadPool::getThreadNum()
	{
		std::lock_guard<std::mutex> lock(mtx_);
		return single_thread_vec_.size();
	}
	void TrantorCachedThreadPool::waitUntilFinished()
	{
		std::lock_guard<std::mutex> lock(mtx_);
		for (auto item : single_thread_vec_)
		{
			item->waitUntilFinished();
		}
	}
	void TrantorCachedThreadPool::reset()
	{
		std::lock_guard<std::mutex> lock(mtx_);
		single_thread_vec_.clear();
		single_thread_vec_.resize(0);
	}
}
