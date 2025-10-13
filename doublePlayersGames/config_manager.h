#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include "manager.h" //����ģ�����

#include "map.h"
#include<SDL.h>
#include <string>
#include <cJSON.h>

#include <fstream>
#include <sstream>
#include <iostream>


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

private:



protected:

	ConfigManager() = default;


	~ConfigManager() = default;

};










#endif // !1
