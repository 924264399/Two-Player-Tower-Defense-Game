#ifndef _WAVE_H
#define _WAVE_H

#include "enemy_type.h"

#include <vector>



///�� level.json ���������Ϣ
struct wave
{

	struct SpawnEvent
	{
		double interval = 0;
		int spawn_point = 1;

		EnemyType enemy_type = EnemyType::Slim; //Ĭ������ʷ��ķ

	};

	double rawards = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;

};







#endif // !1
