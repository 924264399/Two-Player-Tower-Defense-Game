#define SDL_MAIN_HANDLED //防止和sdl定义的main 冲突

#include <iostream>  //输入输出 比如你常用的cout  cin  
#include <string>
#include <fstream>  //读文件的
#include <sstream>   //std::stringstream 流  应用场景：字符串的 “拆分” 与 “格式转换


#include <cJSON.h>
#include <SDL.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>  //基本图元的绘制



void test_json()
{
	std::ifstream file("testjson.json");  //读取文件用的还是 输入流 这个库

	if (!file.good())
	{


		std::cout << "no file" << std::endl;
		return;

	}


	std::stringstream str_stream;
	str_stream << file.rdbuf();   //这是把文件数据全部读到 stringstream str_stream 里    这个stringstream是是 STL 中 <sstream> 头文件提供的字符串流类，可以理解为 “内存中的字符串缓冲区”，兼具 “输入流” 和 “输出流” 的功能：
								//它的核心作用是在内存中临时存储和处理字符串，方便进行字符串拼接、格式转换（如数字转字符串）等操作
	file.close();               //为什么要又这么个 读取 - 转移数据  -关闭文件 的流程？ file.close() 可以手动释放文件句柄，避免文件被长时间锁定（比如其他程序想修改该文件时会失败）

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str()); //解析已经包含所有数据的str_stream的字符串 和 c_str  就是把字符串解析为json的语法结构
															//这个cJSON你进去看 会发现包含了所有的常用数据类型 这是用一个结构体兼容 JSON 的所有数据类型，解析时根据 type 判断该用哪个 value 成员，从而正确存储和访问 JSON 中的数据。



	cJSON* json_name = cJSON_GetObjectItem(json_root, "xingming");//从json_root里找 name
	cJSON* json_age = cJSON_GetObjectItem(json_root, "nianling");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "chongwu");


	std::cout << json_name->string << ":" << json_name->valuestring << std::endl;  // 这里的 ->string是key  ->valuestring 是value  具体你要去cJSON这个struct里看

	std::cout << json_name->string << ":" << json_name->valueint << std::endl;
	std::cout << json_pets->string << ":" <<  std::endl;


	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item, json_pets)   //因为在我提供json文件里 json_pets读取的是chongwu 这个列表  所以这边要进行遍历的  cJSON_ArrayForEach打开这个函数你会发现是个for循环
	{

		std::cout << "\t" << json_item->valuestring << std::endl;

	}



}




void test_csv()
{

	std::ifstream file("test.csv");

	if (!file.good())
	{
		std::cout << "no file" << std::endl;
		return;
	}

	std::string str_line;							//// 存储"一行文本"的字符串

	while (std::getline(file, str_line))          // 循环读文件的每一行（用getline默认分隔符'\n'）
	{
		std::string str_grid;						//存储"一个CSV字段"的字符串
		std::stringstream str_stream (str_line);   //把"一行文本"转换成"内存中的字符串流"（因为getline只能读流）  std::stringstream是库sstream的函数 用于字符串格式准换然后拆分啥的
		
		while (std::getline(str_stream, str_grid,','))   //把str_stream 读到 str_grid
		{

			std::cout << str_grid << " ";


		}

		std::cout << std::endl;

	}

	file.close();
}




int main()
{

	test_json();
	test_csv();



	SDL_Init(SDL_INIT_EVERYTHING);                               //初始化所有系统
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);                       //图片的化先初始这两个格式
	Mix_Init(MIX_INIT_MP3);                                      //音频库的化初始化MP3格式
	TTF_Init();                                                  // 初始化字体


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);            //打开音频文件  参数（音频采样率 一般是44100hz，格式比如mp3 ,通道数 立体声就是双通道就是2，音频缓冲区大小 大的缓冲区 性能消耗小 但是延迟高 小的缓冲区相反）

	SDL_Window* window = SDL_CreateWindow(u8"你好世界",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);  //创建游戏窗口
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);                                             //简单的渲染器 参数（窗口，初始化的驱动索引index,硬件加速或者垂直同步等设置）


	SDL_Surface* suf_img = IMG_Load("image2.jpg");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);                                                      //要渲染一个图片都是这个步骤  先读取图片到内存  SDL_CreateTextureFromSurface则负责把图从cpu-GPU 变成纹理

	TTF_Font* font = TTF_OpenFont("ipix.ttf",32);																				//打开字体文件（是的 你需要一个字体文件）
	SDL_Color color = { 255,255,255,255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"你好，世界",color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);                                                    //字体一样的 要变成纹理 即surface到texture

	//--------------SDL_Surface是存储于​​系统内存（CPU可访问）​​ 中的像素数据（如RGBA格式），可以直接通过CPU修改像素
	//--------------SDL_Texture是存储于​​显存（GPU专用）​​ 中的图像对象 ​​不可直接修改​​：无法通过CPU直接访问像素数据，需通过Surface创建或更新
	//--------------Texture通常由Surface转换而来  然后还需要一个绘制的位置和宽高 这就由SDL_Rect结构体完成  
	//--------------最后的渲染函数 譬如SDL_RenderCopy 既需要SDL_Texture   SDL_Rect  这就是图片字体渲染走的链


	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);   //-1是一直循环 持续1500毫秒

	int fps = 60; //帧率




	bool is_quit = false;                                                                                                       //程序是否退出的标志位

	SDL_Event event;                                                                                                            //实例化这个 才能拖拽窗口啥的操作 处理各种事件 比如鼠标的各种操作 窗口的缩放拖拽 关闭窗口等
	SDL_Point pos_cursor = { 0,0 };
	SDL_Rect rect_img, rect_text;

	Uint64 last_counter = SDL_GetPerformanceCounter();           //这是高性能计数器的计数总数
	Uint64 conter_freq = SDL_GetPerformanceFrequency();          //频率 每一秒这个高性能计数器会跳多少下



	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;  //要渲染图片的位置 宽高的信息


	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;  //要渲染的字体的位置 宽高的信息



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


		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / conter_freq; // (当前计数 - 上一次计数)/频率   就可以得到这一次调用 和 上次调用之间的秒数(一次循环的秒数)

		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / 60)
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));   //这是舍弃什么？




		//处理数据模块 游戏逻辑层
		rect_img.x = pos_cursor.x; //这里是显示图片的位置宽高啥的
		rect_img.y = pos_cursor.y;


		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;


		//渲染模块
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        //设置renderer渲染颜色为黑色
		SDL_RenderClear(renderer);                             //用renderer 覆盖整个窗口  这时候renderer是黑色的  就是清屏了

		
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);//最后一个参数就是要提供渲染的四个分量 （x，y）确认在哪里位置  （z，w）确认绘制多大

		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 25, 25, 128); //画个圆。。。。

		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);

		SDL_RenderPresent(renderer);                         //不断更新渲染画面

	}






	return 0;

}