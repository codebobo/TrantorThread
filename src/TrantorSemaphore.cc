#include "TrantorSemaphore.h"

namespace trantor
{
	void TrantorSemaphore::wait()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		cv_.wait(lock, [=]() {return avaliable_res_num_ > 0; });
		avaliable_res_num_--;
	}
	void TrantorSemaphore::post()
	{
		{
			std::unique_lock<std::mutex> lock(mtx_);
			if (avaliable_res_num_ < max_res_num_)
			{
				avaliable_res_num_++;
			}
		}
		cv_.notify_all();
	}
	uint16_t TrantorSemaphore::getResNum()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return avaliable_res_num_;
	}
}