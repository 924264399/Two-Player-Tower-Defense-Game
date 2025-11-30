#ifndef _SLIM_ENEMY_H_
#define _SLIM_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"  //资源管理的


class SlimEnemy : public Enemy   //继承敌人类
{
public:

	SlimEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();		//先拿到到资源管理器的纹理池（只读）

		static SDL_Texture* tex_slime = texture_pool.find(ResID::Tex_Slime)->second;     //找到对应的纹理 这个是资源管理器那边先加载的 
																						//资源管理器中的定义：TexturePool 是「键为 ResID，值为 SDL_Texture*」的哈希表
																						//second 就是取键值对中的「值」
			
		static SDL_Texture* tex_slime_sketch = texture_pool.find(ResID::Tex_SlimeSketch)->second;  //专属的剪影纹理 （受到打击）
		static ConfigManager::EnemyTemplate& slim_template = ConfigManager::instance()->slim_template; //取出配置管理器里的史莱姆专属数值模板（血量、速度等预设）


		//动画
		static const std::vector<int> idx_list_up = { 6, 7, 8, 9, 10, 11 };  // 朝上走的6帧
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };		//朝下走的6帧
		static const std::vector<int> idx_list_left = { 18, 19, 20, 21, 22, 23 };	//朝左走的6帧
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };	//朝右走的6帧


		//具有的动画配置
		anim_up.set_loop(true); anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_right);


		//把上面拿到的配置管理器里的史莱姆数值 进行填充  这里能直接填充是因为父类Enemy已经定义过
		max_hp = slim_template.hp;
		max_speed = slim_template.speed;
		damage = slim_template.damage;
		reward_ratio = slim_template.reward_ratio;
		recover_interval = slim_template.recover_interval;
		recover_range = slim_template.recover_range;
		recover_intensity = slim_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;


	}


	~SlimEnemy() = default;


};







#endif // !_SLIM_ENEMY_H_