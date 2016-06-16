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

	trantor::TrantorAny a = 1;
	std::string name = a.typeId().name();
	auto b = GET_ANY_VALUE(int, a);
	return 0;
}