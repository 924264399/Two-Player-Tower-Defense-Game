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


		}


	}



	bool load_game_config(const std::string& path)
	{



	}




protected:

	ConfigManager() = default;


	~ConfigManager() = default;

};










#endif // !1
