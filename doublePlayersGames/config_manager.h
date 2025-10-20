#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include "manager.h" //����ģ�����

#include "map.h"
#include"wave.h"


#include<SDL.h>
#include <string>
#include <cJSON.h>

#include <fstream>
#include <sstream>
#include <iostream>


////�����ļ����ǽ��� �� ���ùؿ����ε�һϵ�����Ե� 


class ConfigManager : public Manager<ConfigManager>
{

	friend class Manager<ConfigManager>; //��Manager �ܷ����Լ��� private �� protectedȨ��

public:

	///���� ���� �ӻ��������� ���ڴ�С �� ������ �� ���˵ĳ�ʼ���Եȵ�
	struct BssicTemplate
	{
		std::string window_title = u8"�����ѣ�";
		int window_width =1280;
		int window_height = 720;

	};


	struct PlayerTemplate
	{
		double speed = 3;
		double normal_attack_interval = 0.5;
		double normal_attack_damage = 0;
		double skill_interval = 10;
		double skill_damage = 1;



	};

	struct TowerTemplate
	{
		double interval[10] = { 1 };
		double damage[10] = { 25 };
		double view_range[10] = { 5 };
		double cost[10] = { 50 };
		double upgrade_cost[9] = { 75 };

	};


	struct EnemyTemplate
	{
		double hp = 100;
		double speed = 1;
		double damage = 1;
		double reward_ratio = 0.5;
		double recover_interval = 10;
		double recover_range = 0;
		double recover_intensity = 25;

	};



public:
	Map map;
	std::vector<Wave> wave_list;

	int level_archer = 0;
	int level_axeman = 0;
	int level_gunner = 0;

	//��Ϸ�ǲ���ʤ�����߽���
	bool is_game_win = true;
	bool is_game_over = false;
	SDL_Rect rect_tilp_map = { 0 };
	 
	BssicTemplate basic_template;
	PlayerTemplate player_template;

	//�������������� ���� axeman ǹ��
	TowerTemplate archer_template;
	TowerTemplate axeman_template;
	TowerTemplate gunner_template;

	//������� ʷ��ķ
	EnemyTemplate slim_template;
	EnemyTemplate king_slim_template;		//�ʼ�ʷ��ķ
	EnemyTemplate skeleton_template;		//����
	EnemyTemplate goblin_template;			//�粼��
	EnemyTemplate goblin_priset_template;  //�粼�ּ���

	//һЩ��ʼ����
	const double num_initial_hp = 10;
	const double num_initial_coin = 100;
	const double num_coin_per_prop = 10;


public:

	bool load_level_config(const std::string& path) //�������һ���ļ� ������ܿ�������Ϣ��json
	{
		//һϵ�в��� ���ļ� ��ת�����洢���ַ������ࣩ �ر��ļ� ���� ���Ը�ֵ 

		std::ifstream file(path);

		if (!file.good()) return false;

		std::stringstream str_stream;

		str_stream << file.rdbuf();  //��������
		
		file.close();

		////��ʼ����
		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root)return false;		//�ж��Ƿ�����ɹ�

		if (json_root->type != cJSON_Array) //���������������
		{
			cJSON_Delete(json_root);
			return false;
		}

		cJSON* json_wave = nullptr;
		cJSON_ArrayForEach(json_wave, json_root) //����json_root �ŵ� json_wave��
		{
			if (json_wave->type != cJSON_Object)//�����ͼ�� �����һ��ѭ���е�json wave�����Ͳ���cJSON_Object �Ǿ��������ѭ�� ������һѭ��
				continue;

			wave_list.emplace_back();
			Wave& wave = wave_list.back();

			cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");   //rawards��wave������
			if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)		//���
				wave.rawards = json_wave_rewards->valuedouble;


			cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");   //intervalҲ����wave������
			if (json_wave_interval && json_wave_interval->type == cJSON_Number)
				wave.interval = json_wave_interval->valuedouble;


			cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list"); //spawn_list��������Ǹ����� ���л�ȡ����֮����Ҫ���б���
			if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array)
			{
				cJSON* json_spawn_event = nullptr;
				cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list)   //����json_wave_spawn_list  �洢��json_spawn_event
				{
					if (json_spawn_event->type != cJSON_Object)
						continue;


					wave.spawn_event_list.emplace_back();
					Wave::SpawnEvent& spawn_event = wave.spawn_event_list.back();

					cJSON* json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval"); //spawn_list �������Ľṹ��ÿ��Ԫ�ض���interval(double) point(int) enemy(enemy�Ƚ��鷳 �Ǹ��ֵ�һ���Ľ�� ��key ��value)
					if (json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number)
						spawn_event.interval = json_spawn_event_interval->valuedouble;


					cJSON* json_spawn_event_spawn_point = cJSON_GetObjectItem(json_spawn_event, "point");
					if (json_spawn_event_spawn_point && json_spawn_event_spawn_point->type == cJSON_Number)
						spawn_event.spawn_point = json_spawn_event_spawn_point->valueint;


					cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
					if (json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String)
					{
						const std::string str_emery_type = json_spawn_event_enemy_type->valuestring;     //valuestring������ֵ�ṹ�� value����������

						if (str_emery_type == "Slim")
							spawn_event.enemy_type = EnemyType::Slim;
						else if (str_emery_type == "KingSlim")
							spawn_event.enemy_type = EnemyType::KingSlim;
						else if (str_emery_type == "Skeleton")
							spawn_event.enemy_type = EnemyType::Skeleton;
						else if (str_emery_type == "Goblin")
							spawn_event.enemy_type = EnemyType::Goblin;
						else if (str_emery_type == "GoblinPriest")
							spawn_event.enemy_type = EnemyType::GoblinPriest;
					

					}

				}

				if (wave.spawn_event_list.empty())
					wave_list.pop_back();    //����Ϊ�˷�ֹ ����������spawn_list���� ���ǿյ� ��������������Ĳ��Ϸ�
			}

		}

		cJSON_Delete(json_root);  //�ͷ�һ��

		if (wave_list.empty())
			return false;          //����ⶼ�ǿյ�  ��˵���ļ����ļ� ֱ��false

		return true;
	}



	bool load_game_config(const std::string& path)
	{



	}




protected:

	ConfigManager() = default;


	~ConfigManager() = default;

};










#endif // !1
