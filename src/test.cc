#include<iostream>
#include <utility>
#include <functional>
#include "TrantorThreadPool.h"
#include "TrantorBlockQueue.h"
#include "TrantorSemaphore.h"
#include "TrantorShareMutex.h"

using namespace std;
using namespace trantor;

void trantorThreadPoolTest()
{
	TrantorCachedThreadPool pool(10);
	for (int i = 0; i < 10000; i++)
	{
		pool.pushTask([]() {int i = 0; });
	}
	pool.waitUntilFinished();
	cout << "thread num1: " << pool.getThreadNum() << endl;
	pool.reset();
	cout << "thread num2: " << pool.getThreadNum() << endl;
	for (int i = 0; i < 10000; i++)
	{
		pool.pushTask([]() {int i = 0; });
	}
	pool.waitUntilFinished();
	cout << "thread num3: " << pool.getThreadNum() << endl;
}

void push(TrantorBlockQueue<int>* queue)
{
	for (int i = 0; i < 100; i++)
	{
		queue->put(i);
		cout << "push!queue size: " << queue->size() << endl;
	}
}

void take(TrantorBlockQueue<int>* queue)
{
	for (int i = 0; i < 100; i++)
	{
		queue->take();
		cout << "take!queue size: " << queue->size() << endl;
	}
}

void trantorBlockQueueTest()
{
	TrantorBlockQueue<int> queue(50);
	std::thread push_thread(std::bind(push, &queue));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::thread take_thread(std::bind(take, &queue));
	push_thread.join();
	take_thread.join();
}

void sema_wait(TrantorSemaphore* sema)
{
	cout << "wait" << endl;
	sema->wait();
	cout << "wait" << endl;
	sema->wait();
	cout << "wait" << endl;
	sema->wait();
	cout << "wait" << endl;
	sema->wait();
	cout << "wait" << endl;
}

void sema_post(TrantorSemaphore* sema)
{
	cout << "post" << endl;
	sema->post();
}

void trantorSemaphoreTest()
{
	TrantorSemaphore sema(3);
	std::thread wait_thread(bind(sema_wait, &sema));
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::thread post_thread(bind(sema_post, &sema));
	wait_thread.join();
	post_thread.join();
}

void rd_thread(TrantorShareMutex* mtx)
{	
	TrantorRdLock lock(*mtx);
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void wr_thread(TrantorShareMutex* mtx)
{
	for (int i = 0; i < 10; i++)
	{
		TrantorWrLock lock(*mtx);
	}
}

void trantorShareMutexTest()
{
	TrantorShareMutex mtx;
	std::thread* rd_test[10];
	for (int i = 0; i < 10; i++)
	{
		rd_test[i] = new std::thread(bind(rd_thread, &mtx));
	}
	std::thread wr_test(bind(wr_thread, &mtx));
	for (int i = 0; i < 10; i++)
	{
		rd_test[i]->join();
	}
	wr_test.join();
}