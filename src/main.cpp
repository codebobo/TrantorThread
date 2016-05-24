#include <vector>
#include <functional>
extern void trantorThreadPoolTest();
extern void trantorBlockQueueTest();
extern void trantorSemaphoreTest();


int main()
{
	//trantorThreadPoolTest();
	//trantorBlockQueueTest();
	trantorSemaphoreTest();
	return 0;
}