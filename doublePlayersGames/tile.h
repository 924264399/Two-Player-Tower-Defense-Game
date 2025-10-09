#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

#define SIZE_TILE 48 //48 flag

struct Tile
{
	enum Direction
	{
		None = 0,
		Up,
		Down,
		Lift,
		Right,
	};


	int terrian = 0;
	int decoration = -1;
	int special_flag = -1;  //特殊标志 家和出身点
	bool has_tower = false;  //判断当前位置块是否放置防御塔的

	Direction direction = Direction::None;

};

typedef std::vector<std::vector<Tile>> TileMap; //typedef 关键字 起别名  std::vector<std::vector<Tile>>是二维动态数组




#endif //!_TILE_H_
