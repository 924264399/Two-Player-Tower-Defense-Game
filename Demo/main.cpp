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

	SDL_Window* window = SDL_CreateWindow(u8"你好世界",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);  //创建游戏窗口
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);                                             //简单的渲染器 参数（窗口，初始化的驱动索引index,硬件加速或者垂直同步等设置）


	SDL_Surface* suf_img = IMG_Load("image2.jpg");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);                                                      //要渲染一个图片都是这个步骤  先读取图片到内存  SDL_CreateTextureFromSurface则负责把图从cpu-GPU 变成纹理




	bool is_quit = false;                                                                                                       //程序是否退出的标志位

	SDL_Event event;                                                                                                            //实例化这个 才能拖拽窗口啥的操作 处理各种事件 比如鼠标的各种操作 窗口的缩放拖拽 关闭窗口等
	SDL_Point pos_cursor = { 0,0 };
	SDL_Rect rect_img;

	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;



	while (!is_quit)
	{
		//窗口操作层
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)//如果主动关闭窗口 会触发一个SDL_QUIT事件
			{
				is_quit = true;

			}

			else if (event.type == SDL_MOUSEMOTION)//鼠标移动事件
			{
				pos_cursor.x = event.motion.x; //鼠标移动就更新位置
				pos_cursor.y = event.motion.y;

			}


		}

		//处理数据模块 游戏逻辑层
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;



		//渲染模块
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        //设置renderer渲染颜色为黑色
		SDL_RenderClear(renderer);                             //用renderer 覆盖整个窗口  这时候renderer是黑色的  就是清屏了

		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);//最后一个参数就是要提供渲染的四个分量 （x，y）确认在哪里位置  （z，w）确认绘制多大
		SDL_RenderPresent(renderer);                         //不断更新渲染画面

	}






	return 0;

}