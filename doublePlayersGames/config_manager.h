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
	struct BasicTemplate
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
	 
	BasicTemplate basic_template;
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
	EnemyTemplate goblin_priest_template;  //哥布林祭祀

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

			cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");   //rawards是wave的属性
			if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)		//检查
				wave.rawards = json_wave_rewards->valuedouble;


			cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");   //interval也是是wave的属性
			if (json_wave_interval && json_wave_interval->type == cJSON_Number)
				wave.interval = json_wave_interval->valuedouble;


			cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list"); //spawn_list这个属性是个数组 所有获取到了之后还需要进行遍历
			if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array)
			{
				cJSON* json_spawn_event = nullptr;
				cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list)   //遍历json_wave_spawn_list  存储到json_spawn_event
				{
					if (json_spawn_event->type != cJSON_Object)
						continue;


					wave.spawn_event_list.emplace_back();
					Wave::SpawnEvent& spawn_event = wave.spawn_event_list.back();

					cJSON* json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval"); //spawn_list 这个数组的结构是每个元素都有interval(double) point(int) enemy(enemy比较麻烦 是个字典一样的结果 有key 和value)
					if (json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number)
						spawn_event.interval = json_spawn_event_interval->valuedouble;


					cJSON* json_spawn_event_spawn_point = cJSON_GetObjectItem(json_spawn_event, "point");
					if (json_spawn_event_spawn_point && json_spawn_event_spawn_point->type == cJSON_Number)
						spawn_event.spawn_point = json_spawn_event_spawn_point->valueint;


					cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
					if (json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String)
					{
						const std::string str_emery_type = json_spawn_event_enemy_type->valuestring;     //valuestring是这个字典结构的 value的数据类型

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
					wave_list.pop_back();    //这是为了防止 第三个属性spawn_list数组 都是空的 导致整个最后填充的不合法
			}

		}

		cJSON_Delete(json_root);  //释放一下

		if (wave_list.empty())
			return false;          //如果这都是空的  那说明文件有文件 直接false

		return true;
	}



	bool load_game_config(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.good()) return false;

		std::stringstream str_stream;
		str_stream << file.rdbuf(); file.close();

		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root || json_root->type != cJSON_Object) return false;

		cJSON* json_basic = cJSON_GetObjectItem(json_root, "basic");
		cJSON* json_player = cJSON_GetObjectItem(json_root, "player");
		cJSON* json_tower = cJSON_GetObjectItem(json_root, "tower");
		cJSON* json_enemy = cJSON_GetObjectItem(json_root, "enemy");

		if (!json_basic || !json_player || !json_tower || !json_enemy
			|| json_basic->type != cJSON_Object
			|| json_player->type != cJSON_Object
			|| json_tower->type != cJSON_Object
			|| json_enemy->type != cJSON_Object)
		{

			cJSON_Delete(json_root);    //记得释放
			return false;

		}

		parse_basic_template(basic_template, json_basic);          ////把数据一层一层拨开  最后放到设置好的对象里

		parse_player_template(player_template, json_player);

		parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
		parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
		parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));

		parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
		parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
		parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
		parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
		parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest")); 

		cJSON_Delete(json_root);  //最后一定释放
		return true;


	}




protected:

	ConfigManager() = default;


	~ConfigManager() = default;


private:
	void parse_basic_template(BasicTemplate& tpl,cJSON* json_root)
		{
		if (!json_root || json_root->type != cJSON_Object) return;

		cJSON* json_window_title = cJSON_GetObjectItem(json_root, "window_title");
		cJSON* json_window_width = cJSON_GetObjectItem(json_root, "window_width");
		cJSON* json_window_height = cJSON_GetObjectItem(json_root, "window_height");

		if (json_window_title && json_window_title->type == cJSON_String)    //如果存在  并且 类型正确   才赋值
			tpl.window_title = json_window_title->valuestring;

		if (json_window_width && json_window_width->type == cJSON_Number)
			tpl.window_width = json_window_width->valueint;

		if (json_window_height && json_window_height->type == cJSON_Number)
			tpl.window_height = json_window_height->valueint;
		}



	void parse_player_template(PlayerTemplate& tpl, cJSON* json_root)      //这是玩家的属性
		{
		if (!json_root || json_root->type != cJSON_Object) return;

		cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
		cJSON* json_normal_attack_interval = cJSON_GetObjectItem(json_root, "normal_attack_interval");
		cJSON* json_normal_attack_damage = cJSON_GetObjectItem(json_root, "normal_attack_damage");
		cJSON* json_skill_interval = cJSON_GetObjectItem(json_root, "skill_interval");
		cJSON* json_skill_damage = cJSON_GetObjectItem(json_root, "skil_damage");

		if (json_speed && json_speed->type == cJSON_Number)
			tpl.speed = json_speed->valuedouble;

		if (json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
			tpl.normal_attack_interval = json_normal_attack_interval->valuedouble;

		if (json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
			tpl.normal_attack_damage = json_normal_attack_damage->valuedouble;

		if (json_skill_interval && json_skill_interval->type == cJSON_Number)
			tpl.skill_interval = json_skill_interval->valuedouble;


		if (json_skill_damage && json_skill_damage->type == cJSON_Number)
			tpl.skill_damage = json_skill_damage->valuedouble;
		}


	void parse_number_array(double* ary, int max_len, cJSON* json_root)      //解析一个由数组构成的数字
	{
		if (!json_root || json_root->type != cJSON_Array)					//这里的json_root就是每个数组属性了 所以先检查一下
			return;

		int idx = -1;
		cJSON* json_element = nullptr;
		cJSON_ArrayForEach(json_element, json_root)
		{
			idx++;
			if (json_element->type != cJSON_Number || idx >= max_len)
				continue;

			ary[idx] = json_element->valuedouble;
		}
	}

	void parse_tower_template(TowerTemplate& tpl, cJSON* json_root)             //防御塔
	{
		if (!json_root || json_root->type != cJSON_Object) return;

		cJSON* json_interval = cJSON_GetObjectItem(json_root, "interval");
		cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");			//攻击伤害
		cJSON* json_view_range = cJSON_GetObjectItem(json_root, "view_range");    //视野范围
		cJSON* json_cost = cJSON_GetObjectItem(json_root, "cost");                //升级费用
		cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");

		parse_number_array(tpl.interval, 10, json_interval);                   //因为防御塔的每个属性都是一个数组 因为防御塔会升级 所以需要专门搞个解析数组的函数
		parse_number_array(tpl.damage, 10, json_damage);
		parse_number_array(tpl.view_range, 10, json_view_range);
		parse_number_array(tpl.cost, 10, json_cost);
		parse_number_array(tpl.upgrade_cost, 9, json_upgrade_cost);
	}


	void parse_enemy_template(EnemyTemplate& tpl, cJSON* json_root)				//敌人
	{
		if (!json_root || json_root->type != cJSON_Object) return;

		cJSON* json_hp = cJSON_GetObjectItem(json_root, "hp");
		cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
		cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
		cJSON* json_reward_ratio = cJSON_GetObjectItem(json_root, "reward_ratio");
		cJSON* json_recover_interval = cJSON_GetObjectItem(json_root, "recover_interval");
		cJSON* json_recover_range = cJSON_GetObjectItem(json_root, "recover_range");
		cJSON* json_recover_intensity = cJSON_GetObjectItem(json_root, "recover_intensity");

		if (json_hp && json_hp->type == cJSON_Number)
			tpl.hp = json_hp->valuedouble;
		if (json_speed && json_speed->type == cJSON_Number)
			tpl.speed = json_speed->valuedouble;
		if (json_damage && json_damage->type == cJSON_Number)
			tpl.damage = json_damage->valuedouble;
		if (json_reward_ratio && json_reward_ratio->type == cJSON_Number)
			tpl.reward_ratio = json_reward_ratio->valuedouble;
		if (json_recover_interval && json_recover_interval->type == cJSON_Number)
			tpl.recover_interval = json_recover_interval->valuedouble;
		if (json_recover_range && json_recover_range->type == cJSON_Number)
			tpl.recover_range = json_recover_range->valuedouble;
		if (json_recover_intensity && json_recover_intensity->type == cJSON_Number)
			tpl.recover_intensity = json_recover_intensity->valuedouble;
	}


};










#endif // !1
