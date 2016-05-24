#include <vector>
#include <functional>
extern void trantorThreadPoolTest();
extern void trantorBlockQueueTest();

void test(std::vector<int>& tmp)
{}

int main()
{
	std::vector<int> a;
	auto f = std::bind(test, a);
	//trantorThreadPoolTest();
	trantorBlockQueueTest();
	return 0;
}