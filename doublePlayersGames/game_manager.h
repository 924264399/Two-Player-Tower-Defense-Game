#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"


class GameManager : public Manager<GameManager>    //这个写法是模板的写法
{

	friend class Manager<GameManager>; //友元 主要是父类Manager要访问子类GameManager的私有的构造


public:



protected:

	GameManager()
	{
	}

	~GameManager()
	{

	}


};



#endif










