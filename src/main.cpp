#include<map>
#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include<memory>
#include <functional>
#include <utility>
#include "TrantorBlockingQueue.h"
#include "TrantorThreadPool.h"

using namespace std;
using namespace trantor;

int main()
{
	TrantorCachedThreadPool pool(10);
	for (int i = 0; i < 10000; i++)
	{
		pool.pushTask([]() {int i = 0; });
	}
	pool.waitUntilFinished();
	cout <<"thread num1: "<< pool.getThreadNum() << endl;
	pool.reset();
	cout << "thread num2: " << pool.getThreadNum() << endl;
	for (int i = 0; i < 10000; i++)
	{
		pool.pushTask([]() {int i = 0; });
	}
	pool.waitUntilFinished();
	cout << "thread num3: " << pool.getThreadNum() << endl;
	return 0;
}