#define SDL_MAIN_HANDLED //防止和sdl定义的main 冲突

#include <iostream>
#include <SDL.h>
#include<SDL_image.h>

int main()
{
	std::cout << "hellow demo" << std::endl;

	return 0;

}