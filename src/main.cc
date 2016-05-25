#include <vector>
#include <functional>
#include <mutex>
extern void trantorThreadPoolTest();
extern void trantorBlockQueueTest();
extern void trantorSemaphoreTest();
extern void trantorShareMutexTest();

int main()
{
	//trantorThreadPoolTest();
	//trantorBlockQueueTest();
	//trantorSemaphoreTest();
	trantorShareMutexTest();
	std::mutex mtx;
	//mtx.lock();
	//mtx.lock();
	return 0;
}