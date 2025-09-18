#define SDL_MAIN_HANDLED //��ֹ��sdl�����main ��ͻ

#include <iostream>
#include <SDL.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include<SDL_image.h>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);                               //��ʼ������ϵͳ
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);                       //ͼƬ�Ļ��ȳ�ʼ��������ʽ
	Mix_Init(MIX_INIT_MP3);                                      //��Ƶ��Ļ���ʼ��MP3��ʽ
	TTF_Init();                                                  // ��ʼ������


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);            //����Ƶ�ļ�  ��������Ƶ������ һ����44100hz����ʽ����mp3 ,ͨ���� ����������˫ͨ������2����Ƶ��������С ��Ļ����� ��������С �����ӳٸ� С�Ļ������෴��

	SDL_Window* window = SDL_CreateWindow(u8"�������",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);  //������Ϸ����
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);                                             //�򵥵���Ⱦ�� ���������ڣ���ʼ������������index,Ӳ�����ٻ��ߴ�ֱͬ�������ã�


	SDL_Surface* suf_img = IMG_Load("image2.jpg");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);                                                      //Ҫ��Ⱦһ��ͼƬ�����������  �ȶ�ȡͼƬ���ڴ�  SDL_CreateTextureFromSurface�����ͼ��cpu-GPU �������




	bool is_quit = false;                                                                                                       //�����Ƿ��˳��ı�־λ

	SDL_Event event;                                                                                                            //ʵ������� ������ק����ɶ�Ĳ��� ��������¼� �������ĸ��ֲ��� ���ڵ�������ק �رմ��ڵ�
	SDL_Point pos_cursor = { 0,0 };
	SDL_Rect rect_img;

	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;



	while (!is_quit)
	{
		//���ڲ�����
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)//��������رմ��� �ᴥ��һ��SDL_QUIT�¼�
			{
				is_quit = true;

			}

			else if (event.type == SDL_MOUSEMOTION)//����ƶ��¼�
			{
				pos_cursor.x = event.motion.x; //����ƶ��͸���λ��
				pos_cursor.y = event.motion.y;

			}


		}

		//��������ģ�� ��Ϸ�߼���
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;



		//��Ⱦģ��
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        //����renderer��Ⱦ��ɫΪ��ɫ
		SDL_RenderClear(renderer);                             //��renderer ������������  ��ʱ��renderer�Ǻ�ɫ��  ����������

		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);//���һ����������Ҫ�ṩ��Ⱦ���ĸ����� ��x��y��ȷ��������λ��  ��z��w��ȷ�ϻ��ƶ��
		SDL_RenderPresent(renderer);                         //���ϸ�����Ⱦ����

	}






	return 0;

}