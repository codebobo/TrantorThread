#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "TrantorNonCopyable.h"

namespace trantor
{
	class TrantorSemaphore : TrantorNonCopyable
	{
	public:
		explicit TrantorSemaphore(const uint16_t max_res_num) :max_res_num_(max_res_num), avaliable_res_num_(max_res_num){}
		void wait();
		void post();
		uint16_t getResNum();
	private:
		std::mutex mtx_;
		std::condition_variable cv_;
		std::atomic<uint16_t> max_res_num_;
		std::atomic<uint16_t> avaliable_res_num_;
	};
}

