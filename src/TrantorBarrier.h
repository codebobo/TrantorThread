#pragma once
#include <mutex>
#include <condition_variable>
#include "TrantorNonCopyable.h"
namespace trantor
{
	class TrantorBarrier : TrantorNonCopyable
	{
	public:
		explicit TrantorBarrier(const uint32_t barrier_thread_num) 
		:barrier_thread_num_(barrier_thread_num),
		barrier_thread_cnt_(barrier_thread_num),
		reset_flag_(false){}
		void wait();
		void reset();
	private:
		uint32_t barrier_thread_num_;
		uint32_t barrier_thread_cnt_;
		std::mutex mtx_;
		std::condition_variable cv_;
		bool reset_flag_;
	};
}
