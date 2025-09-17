#define SDL_MAIN_HANDLED //防止和sdl定义的main 冲突

#include <iostream>
#include <SDL.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include<SDL_image.h>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);                               //初始化所有系统
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);                       //图片的化先初始这两个格式
	Mix_Init(MIX_INIT_MP3);                                      //音频库的化初始化MP3格式
	TTF_Init();                                                  // 初始化字体


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);            //打开音频文件  参数（音频采样率 一般是44100hz，格式比如mp3 ,通道数 立体声就是双通道就是2，音频缓冲区大小 大的缓冲区 性能消耗小 但是延迟高 小的缓冲区相反）

	SDL_Window* window = SDL_CreateWindow(u8"你好世界",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);  //创建窗口
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);                                             //简单的渲染器 参数（窗口，初始化的驱动索引index,硬件加速或者垂直同步等设置）


	bool is_quit = false;                                                                                                       //程序是否退出的标志位

	while (!is_quit)
	{






	}






	return 0;

}