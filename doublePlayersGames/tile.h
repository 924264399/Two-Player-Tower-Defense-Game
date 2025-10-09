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
	int special_flag = -1;  //�����־ �Һͳ����
	bool has_tower = false;  //�жϵ�ǰλ�ÿ��Ƿ���÷�������

	Direction direction = Direction::None;

};

typedef std::vector<std::vector<Tile>> TileMap; //typedef �ؼ��� �����  std::vector<std::vector<Tile>>�Ƕ�ά��̬����




#endif //!_TILE_H_
