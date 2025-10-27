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



		ConfigManager* config = ConfigManager::instance();  //单例是这么写的  instance是静态方法 是需要这么写的

		init_assert(config->map.load("map.csv"), u8"加载游戏地图失败");                 //注意config不是这里实例化的 而是ConfigManager类内的属性  所以自己的属性调用自己的方法 所以用->
		init_assert(config->load_level_config("level.json"), u8"加载关卡配置失败");
		init_assert(config->load_game_config("config.json"), u8"加载游戏配置失败");  //init_assert 只关心第一个参数是true 还是 false  如果false返回后面的断言



		SDL_Window* window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config->basic_template.window_width, config->basic_template.window_height, SDL_WINDOW_SHOWN);  //创建游戏窗口

		init_assert(window, u8" 游戏窗口初始化失败！");


		 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);             //简单的渲染器 参数（窗口，初始化的驱动索引index,硬件加速或者垂直同步等设置）

		init_assert(renderer, u8" 创建渲染器失败！");


		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"加载游戏资源失败");  //这个加载了贴图啊 mp3啊 各种资源

		init_assert(generate_tile_map_texture(),u8"生成瓦片纹理失败");

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

	SDL_Texture* tex_tile_map = nullptr;

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


	bool generate_tile_map_texture() //用这个来生成瓦片地图texture
	{

	     ////////////////////////////////////////////----------------------------获取地图、瓦片集、尺寸信息----------------------------------//////////////////////////////////////////////////////////
		const Map& map = ConfigManager::instance()->map;  //获取全局唯一的 Map实例   所以要通过ConfigManager转一圈来获取这个唯一对象 且这个地图只读不修改 所以用const

		const TileMap& tile_map = map.get_tile_map();   //拿到的是二维数组信息  这个是map的csv文件里的  每个格子存的是瓦片ID

		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tilp_map;   //整张地图纹理 在窗口上渲染的位置和尺寸


		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;   //从纹理池重 找到 瓦片那个贴图  就是瓦片集大贴图（瓦片集）

		int width_tex_tile_set, height_tex_tile_set;

		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set); ////   查询（获取）瓦片集的宽高
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);   //用整个宽度/单个 得到一行多少个瓦片（比如瓦片大小是32x32，贴图宽512，则一行有16个 SIZE_TILE是单个瓦片的尺寸（48像素）
																							//num_tile_single_line 瓦片集贴图一行能放多少个瓦片，用来后续计算每个瓦片在贴图中的位置




		////////////////////////////////////////////----------------------------创建地图纹理，设置渲染目标----------------------------------//////////////////////////////////////////////////////////
		int width_tex_tile_map, height_tex_tile_map;

		width_tex_tile_map = (int)map.get_width() * SIZE_TILE;    /// 地图纹理的总宽度 = 地图列数 × 单个瓦片宽
		height_tex_tile_map = (int)map.get_height() * SIZE_TILE;   // 地图纹理的总高度 = 地图行数 × 单个瓦片高

		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		if (!tex_tile_map) return false;                           // // 把渲染目标切换到这个“画布”上，后续绘制的内容都会到这个纹理里

		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;


		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, tex_tile_map);    //设置渲染目标是 tex_tile_map


		////////////////////////////////////////////----------------------------遍历地图格子，逐个拼接瓦片---------------------------------//////////////////////////////////////////////////////////
		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];

				const SDL_Rect& rect_dst =
				{
					x * SIZE_TILE, y * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				rect_src =
				{
					(tile.terrian % num_tile_single_line) * SIZE_TILE,
					(tile.terrian / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};
				SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);

				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE, SIZE_TILE
					};
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst =
		{
			idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE,
			SIZE_TILE, SIZE_TILE
		};
		SDL_RenderCopy(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);




		SDL_SetRenderTarget(renderer, nullptr);  //重新把渲染目标设回窗口 这和内存释放一样  因为前面设置看tex_tile_map 所以现在需要重新设回去

		return true;

	}



};



#endif










