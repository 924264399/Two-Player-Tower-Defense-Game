#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <SDL.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include<SDL_image.h>
//#include<SDL2_gfxPrimitives.h>  //����ͼԪ�Ļ���



class GameManager : public Manager<GameManager>    //���д����ģ���д��  ����էһ����Ϊ������̳и��� ����ģ��ʵ�����á����ࡱ ����� ��Ϊÿ�����ඨ�Ƶ�ר�����ࡱ �������ģ�� ����a������b����ʵ��������a�ľ�̬������aͨ����̬������ʵ�ֵ���
{

	friend class Manager<GameManager>; //��Ԫ ��Ҫ�Ǹ���ManagerҪ��������GameManager��˽�еĹ���


public:

	int run(int argc,char** argv)
	{
		Uint64 last_counter =  SDL_GetPerformanceCounter();
		const Uint64 counter_freq = SDL_GetPerformanceFrequency();

		//��Ϸ��ѭ��  ������˳� �Ͳ�����ȡ�¼�
		while (!is_quit)
		{
			
			while (SDL_PollEvent(&event))
				on_input();

			//��̬��ʱ
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_freq; //(��ǰ���� - ��һ�μ���)/Ƶ��   �Ϳ��Եõ���һ�ε��� �� �ϴε���֮�������(һ��ѭ��������)
			last_counter = current_counter;

			if (delta * 1000 < 1000.0 / 60)  //60֡
				SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));   

			//��������
			on_update(delta);




			//���ƻ���
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        //����renderer��Ⱦ��ɫΪ��ɫ
			SDL_RenderClear(renderer);                             //��renderer ������������  ��ʱ��renderer�Ǻ�ɫ��  ����������

			on_render();

			SDL_RenderPresent(renderer);                         //���ϸ�����Ⱦ����


		}

		return 0;

	}



protected:

	GameManager()   // �ڹ��캯���н��г�ʼ��
	{
		//��ĳ�ʼ�����
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING),u8"SDL2 ��ʼ��ʧ�ܣ�");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image ��ʼ��ʧ�ܣ�");
		init_assert(Mix_Init(MIX_INIT_MP3), u8" music ��ʼ��ʧ�ܣ�");
		init_assert(!TTF_Init(), u8" SDL_TTF��ʼ��ʧ�ܣ�");

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // SDL_mixer �⣨SDL ����Ƶ��չ�⣩�ĺ��ĳ�ʼ������   ��ʼ����Ƶͳһ�����ʽ

		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1"); //���뷨�û�������ʾ������  ���� IME �� UI ��ʾ���û��������ĵ���Ҫת��������ʱ��������������ѡ�ʴ��ڣ�����ѡ��



		ConfigManager* config = ConfigManager::instance();  //��������ôд��  instance�Ǿ�̬���� ����Ҫ��ôд��

		init_assert(config->map.load("map.csv"), u8"������Ϸ��ͼʧ��");                 //ע��config��������ʵ������ ����ConfigManager���ڵ�����  �����Լ������Ե����Լ��ķ��� ������->
		init_assert(config->load_level_config("level.json"), u8"���عؿ�����ʧ��");
		init_assert(config->load_game_config("config.json"), u8"������Ϸ����ʧ��");  //init_assert ֻ���ĵ�һ��������true ���� false  ���false���غ���Ķ���



		SDL_Window* window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config->basic_template.window_width, config->basic_template.window_height, SDL_WINDOW_SHOWN);  //������Ϸ����

		init_assert(window, u8" ��Ϸ���ڳ�ʼ��ʧ�ܣ�");


		 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);             //�򵥵���Ⱦ�� ���������ڣ���ʼ������������index,Ӳ�����ٻ��ߴ�ֱͬ�������ã�

		init_assert(renderer, u8" ������Ⱦ��ʧ�ܣ�");


		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"������Ϸ��Դʧ��");  //�����������ͼ�� mp3�� ������Դ

		init_assert(generate_tile_map_texture(),u8"������Ƭ����ʧ��");

	}

	~GameManager()  //�����������н����ͷ�
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
	bool is_quit = false;  //�˳�



	SDL_Window* window = nullptr;  // ���������Ϊ�˱����õ� 
	SDL_Renderer* renderer = nullptr; //��Ⱦ��

	SDL_Texture* tex_tile_map = nullptr;

private:

	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;


		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"��Ϸ����ʧ��",err_msg,window);  //ע���õ���һ������ ���������������ܸ�������Ϸ������֮��
		exit(-1);																	  // C++ ��׼��������������ֹ��ǰ�����ִ��


	}


	void on_input()
	{



	}



	void on_update(double delta) //����Ĳ�����֡���µ��¼�  ����һ֡ �� ��һ֡  ʵ�ʹ�ȥ�˶���ʱ��
	{


	}

	void on_render()
	{



	}


	bool generate_tile_map_texture() //�������������Ƭ��ͼtexture
	{

	     ////////////////////////////////////////////----------------------------��ȡ��ͼ����Ƭ�����ߴ���Ϣ----------------------------------//////////////////////////////////////////////////////////
		const Map& map = ConfigManager::instance()->map;  //��ȡȫ��Ψһ�� Mapʵ��   ����Ҫͨ��ConfigManagerתһȦ����ȡ���Ψһ���� �������ͼֻ�����޸� ������const

		const TileMap& tile_map = map.get_tile_map();   //�õ����Ƕ�ά������Ϣ  �����map��csv�ļ����  ÿ�����Ӵ������ƬID

		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tilp_map;   //���ŵ�ͼ���� �ڴ�������Ⱦ��λ�úͳߴ�


		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;   //��������� �ҵ� ��Ƭ�Ǹ���ͼ  ������Ƭ������ͼ����Ƭ����

		int width_tex_tile_set, height_tex_tile_set;

		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set); ////   ��ѯ����ȡ����Ƭ���Ŀ��
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);   //���������/���� �õ�һ�ж��ٸ���Ƭ��������Ƭ��С��32x32����ͼ��512����һ����16�� SIZE_TILE�ǵ�����Ƭ�ĳߴ磨48���أ�
																							//num_tile_single_line ��Ƭ����ͼһ���ܷŶ��ٸ���Ƭ��������������ÿ����Ƭ����ͼ�е�λ��




		////////////////////////////////////////////----------------------------������ͼ����������ȾĿ��----------------------------------//////////////////////////////////////////////////////////
		int width_tex_tile_map, height_tex_tile_map;

		width_tex_tile_map = (int)map.get_width() * SIZE_TILE;    /// ��ͼ������ܿ�� = ��ͼ���� �� ������Ƭ��
		height_tex_tile_map = (int)map.get_height() * SIZE_TILE;   // ��ͼ������ܸ߶� = ��ͼ���� �� ������Ƭ��

		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		if (!tex_tile_map) return false;                           // // ����ȾĿ���л���������������ϣ��������Ƶ����ݶ��ᵽ���������

		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;


		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, tex_tile_map);    //������ȾĿ���� tex_tile_map


		////////////////////////////////////////////----------------------------������ͼ���ӣ����ƴ����Ƭ---------------------------------//////////////////////////////////////////////////////////
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




		SDL_SetRenderTarget(renderer, nullptr);  //���°���ȾĿ����ش��� ����ڴ��ͷ�һ��  ��Ϊǰ�����ÿ�tex_tile_map ����������Ҫ�������ȥ

		return true;

	}



};



#endif










