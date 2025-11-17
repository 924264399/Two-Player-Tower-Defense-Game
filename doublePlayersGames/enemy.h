
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

	void on_render(SDL_Renderer* renderer) //画血条
	{
		static SDL_Rect rect;
		static SDL_Point point;
		static const int offset_y = 2;
		static const Vector2 size_hp_bar = { 40, 8 };  ///血条的 长40像素 高8像素
		static const SDL_Color color_border = { 116, 185, 124, 255 };   //边框的颜色
		static const SDL_Color color_content = { 226, 255, 194, 255 };  

		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);

		anim_current->on_render(renderer, point);



		//画血条 只有当生命值小于最大生命值的时候 在画
		//怎么画 就是一个线框 + 一个长度在改变的填充矩形
		if (hp < max_hp)
		{
			rect.x = (int)(position.x - size_hp_bar.x / 2);   //首先position.x是敌人的中心位置 那么减去血条的一半宽度 就是x方向上血条的一个坐标
			rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y - offset_y); //一样的 y是敌人的中心y位置 减去一半的高度 - 血条宽度 此时紧贴敌人头顶 再减去一个offest

			rect.w = (int)(size_hp_bar.x * (hp / max_hp)); //宽度随hp变化
			rect.h = (int)size_hp_bar.y;
			SDL_SetRenderDrawColor(renderer, color_content.r, color_content.g, color_content.b, color_content.a);
			SDL_RenderFillRect(renderer, &rect);

			rect.w = (int)size_hp_bar.x;
			SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
	


	void set_on_skill_released(SkillCallback on_skill_released) //技能回调
	{
		this->on_skill_released = on_skill_released;
	}


	//增加hp的函数(有回复技能用)
	void increase_hp(double val)
	{
		hp += val;

		if (hp > max_hp)
			hp = max_hp;
	}


	//减少生命值（首到攻击）
	void decrease_hp(double val)
	{
		hp -= val;

		if (hp <= 0)
		{
			hp = 0;
			is_valid = false;
		}

		is_show_sketch = true;
		timer_sketch.restart();
	}

	//减速
	void slow_down()
	{
		speed = max_speed - 0.5;
		timer_restore_speed.set_wait_time(1);//减速都减1s
		timer_restore_speed.restart();
	}


	//设置位置
	void set_position(const Vector2& position)
	{
		this->position = position;
	}



	//设置路径 生成阶段
	void set_route(const Route* route)
	{
		this->route = route;

		refresh_position_target();
	}


	///让敌人直接死亡的 比如碰到家了
	void make_invalid()
	{
		is_valid = false;
	}


	///获取hp
	double get_hp() const
	{
		return hp;
	}



	//获取位置  碰撞监测用
	const Vector2& get_size() const
	{
		return size;
	}

	//获取位置 碰撞监测用
	const Vector2& get_position() const
	{
		return position;
	}

	//获取速度
	const Vector2& get_velocity() const
	{
		return velocity;
	}

	//碰到家的时候 需要获取这个怪的伤害
	double get_damage() const
	{
		return damage;
	}

	//包金币的概率 被击杀的时候调用
	double get_reward_ratio() const
	{
		return reward_ratio;
	}


	//获取回复半径 要世界坐标
	double get_recover_radius() const
	{
		return SIZE_TILE * recover_range;
	}


	//获取回复强度
	double get_recover_intensity() const
	{
		return recover_intensity;
	}

	//能否被删除  那么就是存活就不能删除  死了的怪物才能删除 所以这里取反
	bool can_remove() const
	{
		return !is_valid;
	}


	//获取这个敌人在这条路径上索敌的进度
	double get_route_process() const
	{

		if (route->get_idx_list().size() == 1)
			return 1;

		return (double)idx_target / (route->get_idx_list().size() - 1);  //从出身点导防守目标的百分比  这个是之后防御塔索敌的时候用  判断优先级别
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
