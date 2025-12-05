#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "enemy.h"
#include "manager.h"
#include "config_manager.h"
//#include "home_manager.h"
#include "slim_enemy.h"
#include "king_slim_enemy.h"

#include "skeleton_enemy.h"
#include "goblin_enemy.h"
#include "goblin_priest_enemy.h"
//#include "bullet_manager.h"
//
//
//#include "coin_manager.h"

#include <vector>
#include <SDL.h>

class EnemyManager : public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;

public:
	typedef std::vector<Enemy*> EnemyList;

public:
	

protected:
	EnemyManager() = default;

	~EnemyManager()
	{
		for (Enemy* enemy : enemy_list)
			delete enemy;
	}

private:
	EnemyList enemy_list;

private:



};

#endif // !_ENEMY_MANAGER_H_

