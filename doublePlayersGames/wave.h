#ifndef _WAVE_H
#define _WAVE_H

#include "enemy_type.h"

#include <vector>



///是 level.json 这里面的信息
struct wave
{

	struct SpawnEvent
	{
		double interval = 0;
		int spawn_point = 1;

		EnemyType enemy_type = EnemyType::Slim; //默认生成史莱姆

	};

	double rawards = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;

};







#endif // !1
