#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include "manager.h" //单例模板基类

#include "map.h"
#include"wave.h"


#include<SDL.h>
#include <string>
#include <cJSON.h>

#include <fstream>
#include <sstream>
#include <iostream>


////整个文件就是解析 和 配置关卡波次的一系列属性的 


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
	std::vector<Wave> wave_list;

	int level_archer = 0;
	int level_axeman = 0;
	int level_gunner = 0;

	//游戏是不是胜利或者结束
	bool is_game_win = true;
	bool is_game_over = false;
	SDL_Rect rect_tilp_map = { 0 };
	 
	BssicTemplate basic_template;
	PlayerTemplate player_template;

	//防御塔几个对象 弓手 axeman 枪手
	TowerTemplate archer_template;
	TowerTemplate axeman_template;
	TowerTemplate gunner_template;

	//怪物对象 史莱姆
	EnemyTemplate slim_template;
	EnemyTemplate king_slim_template;		//皇家史莱姆
	EnemyTemplate skeleton_template;		//骷髅
	EnemyTemplate goblin_template;			//哥布林
	EnemyTemplate goblin_priset_template;  //哥布林祭祀

	//一些初始设置
	const double num_initial_hp = 10;
	const double num_initial_coin = 100;
	const double num_coin_per_prop = 10;


public:

	bool load_level_config(const std::string& path) //传入的是一个文件 即保存管卡波次信息的json
	{
		//一系列操作 打开文件 流转化（存储到字符串流类） 关闭文件 解析 属性赋值 

		std::ifstream file(path);

		if (!file.good()) return false;

		std::stringstream str_stream;

		str_stream << file.rdbuf();  //读到流里
		
		file.close();

		////开始解析
		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root)return false;		//判断是否解析成功

		if (json_root->type != cJSON_Array) //如果不是数组类型
		{
			cJSON_Delete(json_root);
			return false;
		}

		cJSON* json_wave = nullptr;
		cJSON_ArrayForEach(json_wave, json_root) //遍历json_root 放到 json_wave里
		{
			if (json_wave->type != cJSON_Object)//先类型检查 如果这一次循环中的json wave的类型不是cJSON_Object 那就跳过这次循环 进入下一循环
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
