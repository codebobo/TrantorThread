#pragma once

#include <condition_variable>
#include <mutex>

namespace trantor
{
	class TrantorCondition
	{
	public:
		explicit TrantorCondition(std::mutex& mtx):mtx_(mtx) {}
		void wait();
		void notifyOne();
		void notifyAll();
	private:
		std::condition_variable cv_;
		std::mutex& mtx_;
	};
}
