#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timmer
{
public:
	Timmer() =default;
	~Timmer() = default;

private:

	double pass_time = 0.0; //从开始计数过去了多少时间
	double wait_time = 0.0; // 要等待的时间 即过了多少时间启动定时器的功能

	bool pasued = false; //暂停

	bool shotted = false; //超时就是shotted  即触发一次  可以是单词触发  也可以是多次触发  如果是单次的话 就需要一个变量来记录他已经触发过了

	bool one_shot = false; //是否需要单次触发


	std::function<void()> on_timeout;




};




#endif

