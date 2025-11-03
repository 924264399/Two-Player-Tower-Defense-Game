
/////此文件主要实现序列帧动画的实现 主要是导入序列帧动画贴图 然后分割  结合Time.h时间相关函数进行播放。。。
////虽然都是序列帧动画 但是一种是循环播放（比如一直在动的角色）   一种是播放了之后停止（比如爆炸特效）


#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "timer.h"

#include <SDL.h>
#include <vector>
#include <functional>




class Animation
{
public:
	typedef std::function<void()> PlayCallback; //回调函数


public:
	Animation()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout(             //设置回调
			[&]()
			{
				idx_frame++;             //往后推进一帧
				if (idx_frame >= rect_src_list.size())                         //如果说我们的frame到达了最后一个了
				{
					idx_frame = is_loop ? 0 : rect_src_list.size() - 1;       //这时候判断我们的动画是循环播放的还是停止的 来决定是否循环 如果是loop就把idx_frame设置为0 否则就设置为size-1 即保持在最后一个
					if (!is_loop && on_finished)  //如果不是循环播放的 并且回调函数存在 就调用回调函数on_finished
						on_finished();
				}
			}
		);
	}



	~Animation() = default;


private:
	Timer timer;     //实例化时间类
	bool is_loop = true;   //是否循环
	size_t idx_frame = 0;   //帧索引
	PlayCallback on_finished;	//回调函数  定义在上面 typedef std::function<void()> PlayCallback;  主要用于异步操作 比如播放音效需要时间 程序不可能停下来等他播完在去调用某个函数
	SDL_Texture* texture = nullptr;
	std::vector<SDL_Rect> rect_src_list;
	int width_frame = 0, height_frame = 0;




};








#endif // !_ANIMATION_H_
