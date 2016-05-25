#pragma once
#include <mutex>
#include <condition_variable>
#include "TrantorNonCopyable.h"

namespace trantor
{
	class TrantorShareMutex : TrantorNonCopyable
	{
	public:
		explicit TrantorShareMutex()
		:write_count_(0),
		read_count_(0)
		{
		}
		void readLock();
		void writeLock();
		void readUnLock();
		void writeUnLock();
	private:
		uint32_t read_count_;
		uint32_t write_count_;
		std::mutex read_mtx_;
		std::mutex write_mtx_;
		std::condition_variable write_count_zero_cv_;
		std::condition_variable read_count_zero_cv_;
	};

	class TrantorRdLock : TrantorNonCopyable
	{
	public:
		explicit TrantorRdLock(TrantorShareMutex& mtx) : mtx_(mtx){ mtx_.readLock(); }
		~TrantorRdLock() { mtx_.readUnLock(); }
	private:
		TrantorShareMutex& mtx_;
	};
	class TrantorWrLock : TrantorNonCopyable
	{
	public:
		explicit TrantorWrLock(TrantorShareMutex& mtx) : mtx_(mtx) { mtx_.writeLock(); }
		~TrantorWrLock() { mtx_.writeUnLock(); }
	private:
		TrantorShareMutex& mtx_;
	};
}
