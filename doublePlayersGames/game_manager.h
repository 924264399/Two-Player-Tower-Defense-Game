#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"


class GameManager : public Manager<GameManager>    //���д����ģ���д��
{

	friend class Manager<GameManager>; //��Ԫ ��Ҫ�Ǹ���ManagerҪ��������GameManager��˽�еĹ���


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










