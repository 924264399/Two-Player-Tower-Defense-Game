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

	SDL_Window* window = SDL_CreateWindow(u8"�������",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);  //��������
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);                                             //�򵥵���Ⱦ�� ���������ڣ���ʼ������������index,Ӳ�����ٻ��ߴ�ֱͬ�������ã�


	bool is_quit = false;                                                                                                       //�����Ƿ��˳��ı�־λ

	while (!is_quit)
	{






	}






	return 0;

}