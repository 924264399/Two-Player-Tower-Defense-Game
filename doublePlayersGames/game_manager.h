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


		SDL_Window* window = SDL_CreateWindow(u8"���ɣ�������ׯ��", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);  //������Ϸ����

		init_assert(window, u8" ��Ϸ���ڳ�ʼ��ʧ�ܣ�");


		 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);             //�򵥵���Ⱦ�� ���������ڣ���ʼ������������index,Ӳ�����ٻ��ߴ�ֱͬ�������ã�

		init_assert(renderer, u8" ������Ⱦ��ʧ�ܣ�");


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



};



#endif










