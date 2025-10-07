#define SDL_MAIN_HANDLED //防止和sdl定义的main 冲突


#include <iostream>

//#include "manager.h"
#include "game_manager.h"



int main(int argc, char** argv)
{
	return GameManager::instance()->run(argc, argv);


}