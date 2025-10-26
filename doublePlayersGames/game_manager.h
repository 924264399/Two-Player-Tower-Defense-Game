#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <SDL.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include<SDL_image.h>
//#include<SDL2_gfxPrimitives.h>  //基本图元的绘制



class GameManager : public Manager<GameManager>    //这个写法是模板的写法  这样乍一看以为是子类继承父类 但是模板实际上让“父类” 变成了 “为每个子类定制的专属父类” 如果不用模板 子类a和子类b都会实例化父类a的静态方法（a通过静态方法来实现单例
{

	friend class Manager<GameManager>; //友元 主要是父类Manager要访问子类GameManager的私有的构造


public:

	int run(int argc,char** argv)
	{
		Uint64 last_counter =  SDL_GetPerformanceCounter();
		const Uint64 counter_freq = SDL_GetPerformanceFrequency();

		//游戏主循环  如果不退出 就不断拉取事件
		while (!is_quit)
		{
			
			while (SDL_PollEvent(&event))
				on_input();

			//动态延时
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_freq; //(当前计数 - 上一次计数)/频率   就可以得到这一次调用 和 上次调用之间的秒数(一次循环的秒数)
			last_counter = current_counter;

			if (delta * 1000 < 1000.0 / 60)  //60帧
				SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));   

			//更新数据
			on_update(delta);




			//绘制画面
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        //设置renderer渲染颜色为黑色
			SDL_RenderClear(renderer);                             //用renderer 覆盖整个窗口  这时候renderer是黑色的  就是清屏了

			on_render();

			SDL_RenderPresent(renderer);                         //不断更新渲染画面


		}

		return 0;

	}



protected:

	GameManager()   // 在构造函数中进行初始化
	{
		//库的初始化检测
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING),u8"SDL2 初始化失败！");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image 初始化失败！");
		init_assert(Mix_Init(MIX_INIT_MP3), u8" music 初始化失败！");
		init_assert(!TTF_Init(), u8" SDL_TTF初始化失败！");

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // SDL_mixer 库（SDL 的音频扩展库）的核心初始化函数   初始化音频统一输出格式

		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1"); //输入法用户界面显示的配置  启用 IME 的 UI 显示。用户输入中文等需要转换的文字时，会正常弹出候选词窗口，方便选择。


		SDL_Window* window = SDL_CreateWindow(u8"来吧！保卫村庄！", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);  //创建游戏窗口

		init_assert(window, u8" 游戏窗口初始化失败！");


		 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);             //简单的渲染器 参数（窗口，初始化的驱动索引index,硬件加速或者垂直同步等设置）

		init_assert(renderer, u8" 创建渲染器失败！");


	}

	~GameManager()  //在析构函数中进行释放
	{
		SDL_DestroyRenderer(renderer);

		SDL_DestroyWindow(window);

		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();


	}

private:

	SDL_Event event;
	bool is_quit = false;  //退出



	SDL_Window* window = nullptr;  // 这个窗口是为了报错用的 
	SDL_Renderer* renderer = nullptr; //渲染器

private:

	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;


		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"游戏启动失败",err_msg,window);  //注意用的另一个窗口 这样报错弹出来才能覆盖在游戏主界面之上
		exit(-1);																	  // C++ 标准函数用于立即终止当前程序的执行


	}


	void on_input()
	{



	}



	void on_update(double delta) //传入的参数是帧更新的事件  即这一帧 和 上一帧  实际过去了多少时间
	{


	}

	void on_render()
	{



	}



};



#endif










