#include "TrantorShareMutex.h"
#include <iostream>
namespace trantor
{
	void TrantorShareMutex::readLock()
	{
		std::unique_lock<std::mutex> write_lock(write_mtx_);
		write_count_zero_cv_.wait(write_lock, [=]() {return write_count_ == 0; });
		std::lock_guard<std::mutex> read_lock(read_mtx_);
		std::cout << "read lock!read num: " << read_count_ << std::endl;
		std::cout << "read lock!write num: " << write_count_ << std::endl;
		read_count_++;
	}
	void TrantorShareMutex::writeLock()
	{
		std::unique_lock<std::mutex> write_lock(write_mtx_);
		write_count_zero_cv_.wait(write_lock, [=]() {return write_count_ == 0; });
		std::unique_lock<std::mutex> read_lock(read_mtx_);
		read_count_zero_cv_.wait(read_lock, [=]() {return read_count_ == 0; });
		std::cout << "write lock!read num: " << read_count_ << std::endl;
		std::cout << "write lock!write num: " << write_count_ << std::endl;
		write_count_++;
	}
	void TrantorShareMutex::readUnLock()
	{
		std::lock_guard<std::mutex> read_lock(read_mtx_);
		if (read_count_ > 0)
		{
			read_count_--;
			if (read_count_ == 0)
			{
				read_count_zero_cv_.notify_one();
			}
		}
		
	}
	void TrantorShareMutex::writeUnLock()
	{
		std::lock_guard<std::mutex> write_lock(write_mtx_);
		if (write_count_ > 0)
		{
			write_count_--;
			if (write_count_ == 0)
			{
				write_count_zero_cv_.notify_one();
			}
		}
	}
}