#include <vector>
#include <functional>
#include <mutex>
#include "TrantorAny.h"
extern void trantorThreadPoolTest();
extern void trantorBlockQueueTest();
extern void trantorSemaphoreTest();
extern void trantorShareMutexTest();

int main()
{
	//trantorThreadPoolTest();
	//trantorBlockQueueTest();
	//trantorSemaphoreTest();
	//trantorShareMutexTest();
	//std::mutex mtx;
	//mtx.lock();
	//mtx.lock();
	std::vector<int&&> ret;
	for(auto&& a : ret)
	{ }
	return 0;
}