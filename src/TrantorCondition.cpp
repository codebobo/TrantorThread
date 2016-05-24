#include "TrantorCondition.h"

namespace trantor
{
	void TrantorCondition::wait()
	{
		std::unique_lock<std::mutex> lck(mtx_, std::try_to_lock);
		cv_.wait(lck);
	}

	void TrantorCondition::notifyOne()
	{
		cv_.notify_one();
	}

	void TrantorCondition::notifyAll()
	{
		cv_.notify_all();
	}
} 