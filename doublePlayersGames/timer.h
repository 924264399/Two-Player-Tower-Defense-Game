#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
	Timer() =default;
	~Timer() = default;



	void restart()  //状态充值  过去的时间归零  超时也规律
	{
		pass_time = 0;
		shotted = false;
	}



	void set_wait_time(double val) //设置 等待时间的数值
	{
		wait_time = val;
	}


	void set_one_shot(bool flag) //设置是否需要单词触发
	{
		one_shot = false;
	}

	void set_on_timeout(std::function<void()> on_timeout)
	{
		this->on_timeout = on_timeout;
	}

	void pause()    //暂停
	{
		pasued = true;
	}


	void resume()  //恢复
	{
		pasued = false; 
	}

	void on_update(double delta)
	{
		if (pasued) return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();

			pass_time -= wait_time;
		}
	}





private:

	double pass_time = 0.0; //从开始计数过去了多少时间
	double wait_time = 0.0; // 要等待的时间 即过了多少时间启动定时器的功能

	bool pasued = false; //暂停

	bool shotted = false; //超时就是shotted  即触发一次  可以是单词触发  也可以是多次触发  如果是单次的话 就需要一个变量来记录他已经触发过了

	bool one_shot = false; //是否需要单次触发


	std::function<void()> on_timeout;




};




#endif

