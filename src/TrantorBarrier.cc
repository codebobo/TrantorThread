#include "TrantorBarrier.h"

namespace trantor
{
	void TrantorBarrier::wait()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		barrier_thread_cnt_++;
		if (barrier_thread_cnt_ == barrier_thread_num_)
		{
			cv_.notify_all();
		}
		else
		{
			cv_.wait(lock, [=]() {return barrier_thread_cnt_ == barrier_thread_num_; });
		}
	}

	void TrantorBarrier::reset()
	{

	}
}