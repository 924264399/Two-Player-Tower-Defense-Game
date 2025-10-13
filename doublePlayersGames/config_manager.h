#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include "manager.h" //单例模板基类

#include "map.h"
#include<SDL.h>
#include <string>
#include <cJSON.h>

#include <fstream>
#include <sstream>
#include <iostream>


class ConfigManager : public Manager<ConfigManager>
{

	friend class Manager<ConfigManager>; //让Manager 能访问自己的 private 和 protected权限

public:

	///基础 属性 从基础的名字 窗口大小 到 防御塔 和 敌人的初始属性等等
	struct BssicTemplate
	{
		std::string window_title = u8"保卫把！";
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
