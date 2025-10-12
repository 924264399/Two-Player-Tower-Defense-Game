#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "tile.h"

#include<SDL.h>
#include <vector>


///寻路策略
class Route
{

public:
	typedef std::vector<SDL_Point> IdxList;  //这个数组存储的是索引  是路径上每个单元格在地图上的索引


public:
	Route() = default;


	//使用有参构造来
	Route(const TileMap& map, const SDL_Point& idx_origin)
	{
		//地图的长宽的获取
		size_t width_map = map[0].size();
		size_t height_map = map.size();

		SDL_Point idx_next = idx_origin;  //SDL_point 只有两个成员：x（横向位置）和 y（纵向位置），都是整数类型，对应屏幕或纹理上的像素坐标。之后用来记录这个流向信息


		while (true)
		{
			
			if (idx_next.x >= width_map || idx_next.y >= height_map) //如果超出了 地图边界  也退出
				break;

			if (check_duplicate_idx(idx_next)) //遇到重复的单元格的时候会跳出
				break;
			else
				idx_list.push_back(idx_next); //放进去


			bool is_next_dir_exist = true; //下一个方向标

			const Tile& tile = map[idx_next.y][idx_next.x];

			if (tile.special_flag == 0)  //遇到特殊标志了
				break;
			switch (tile.decoration)
			{
			case Tile::Direction::Up:
				idx_next.y--;

			case Tile::Direction::Down:
				idx_next.y++;

			case Tile::Direction::Left:
				idx_next.x--;

			case Tile::Direction::Right:
				idx_next.x++;

			default:
				is_next_dir_exist = false;
				break;

			}

			if (!is_next_dir_exist)   //如果走到头了 下个单元格没有路标了  也跳出
				break;



		}



	}

	~Route() = default;


	const IdxList& get_idx_list() const
	{

		return idx_list;

	}


private:

	IdxList idx_list;

private:

	//检查是否有重复的方法  主要是防止 路径进入死循环 办法是检查下一个要出现的路径是不是已经出现在了idx_list里
	bool check_duplicate_idx(const SDL_Point& target_idx)
	{
		for (const SDL_Point& idx : idx_list)         // 这个写法是c++ 11的  for (A : B) 的核心意思是：依次取出容器 B 中的每一个元素，把它交给 A，然后执行循环体。
		{
			if (idx.x == target_idx.x && idx.y == target_idx.y)  //函数中 idx 是 idx_list 中元素的临时引用，用于遍历列表；target_idx 是传入的目标点，两者对比判断是否重复。
				return true;

		}

		return false;

	}

};




#endif // !_ROUTE_H_

