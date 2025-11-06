
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "timer.h"
#include "route.h"
#include "vector2.h"
#include "animation.h"
#include "config_manager.h"

#include <functional>

class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback;   //回调函数 是哪个敌人释放了技能

public:
	Enemy()  
	{
		timer_skill.set_one_shot(false);							//技能计时器 
		timer_skill.set_on_timeout([&]() { on_skill_released(this); });  //技能计时器 的 回调

		timer_sketch.set_one_shot(true);						//受击之后
		timer_sketch.set_wait_time(0.075);						//手机特效闪的时间
		timer_sketch.set_on_timeout([&]() { is_show_sketch = false; });  //timer_sketch的回调

		timer_restore_speed.set_one_shot(true);								//恢复速度计时器
		timer_restore_speed.set_on_timeout([&]() { speed = max_speed; });   //恢复速度计时器的回调  触发逻辑就是时间到达之后 恢复最大速度
	}

	~Enemy() = default;

	void on_update(double delta)
	{
		timer_skill.on_update(delta);
		timer_sketch.on_update(delta);
		timer_restore_speed.on_update(delta);      //几个计时器 更新一下

		Vector2 move_distance = velocity * delta;						// 速度 * 时间间隔 = 移动juli    算的是我这一帧能移动的最大距离
		Vector2 target_distance = position_target - position;			//前一帧移动的目标距离还有多长
		position += move_distance < target_distance ? move_distance : target_distance;  // 这一帧实际还需要移动的距离

		if (target_distance.approx_zero())   //
		{
			idx_target++;
			refresh_position_target();

			direction = (position_target - position).normalize();  //移动方向
		}

		velocity.x = direction.x * speed * SIZE_TILE;
		velocity.y = direction.y * speed * SIZE_TILE;

		bool is_show_x_amin = abs(velocity.x) >= abs(velocity.y);

		if (is_show_sketch)  //如果显示被击打特效的画  还要做一系列判定  来确定显示什么动画
		{
			if (is_show_x_amin)
				anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
			else
				anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
		}
		else
		{
			if (is_show_x_amin)
				anim_current = velocity.x > 0 ? &anim_right : &anim_left;
			else
				anim_current = velocity.y > 0 ? &anim_down : &anim_up;
		}

		anim_current->on_update(delta);
	}

	

protected:
	Vector2 size;

	Timer timer_skill;  //释放技能的定时器

	Animation anim_up;     //不同的动画
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;  //受击特效动画
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;

	double hp = 0;         //当前生命值
	double max_hp = 0;		//最大生命值
	double speed = 0;			//移动速度
	double max_speed = 0;			//最大移动速度
	double damage = 0;					//怪物对目标的伤害
	double reward_ratio = 0;				
	double recover_interval = 0;		//恢复的时间间隔
	double recover_range = 0;			//恢复的范围
	double recover_intensity = 0;		//恢复的强度

private:
	Vector2 position;    //移动相关的
	Vector2 velocity;	//速度
	Vector2 direction;		//方向

	bool is_valid = true;    //是不是被击杀

	Timer timer_sketch;         //受击特效的 定时器
	bool is_show_sketch = false;

	Animation* anim_current = nullptr;

	SkillCallback on_skill_released;  ////回调函数  

	Timer timer_restore_speed;

	const Route* route = nullptr;
	int idx_target = 0;
	Vector2 position_target;  //要走到的目标位置

private:
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		if (idx_target < idx_list.size())   //还没有走到最后一个位点
		{
			const SDL_Point& point = idx_list[idx_target];  //取出索引
			static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;   ////这里

			position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
			position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
		}
	}

};

#endif // !_ENEMY_H_
