#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "tile.h"

#include<SDL.h>
#include <vector>


///Ѱ·����
class Route
{

public:
	typedef std::vector<SDL_Point> IdxList;  //�������洢��������  ��·����ÿ����Ԫ���ڵ�ͼ�ϵ�����


public:
	Route() = default;


	//ʹ���вι�����
	Route(const TileMap& map, const SDL_Point& idx_origin)
	{
		//��ͼ�ĳ���Ļ�ȡ
		size_t width_map = map[0].size();
		size_t height_map = map.size();

		SDL_Point idx_next = idx_origin;  //SDL_point ֻ��������Ա��x������λ�ã��� y������λ�ã��������������ͣ���Ӧ��Ļ�������ϵ��������ꡣ֮��������¼���������Ϣ


		while (true)
		{
			
			if (idx_next.x >= width_map || idx_next.y >= height_map) //��������� ��ͼ�߽�  Ҳ�˳�
				break;

			if (check_duplicate_idx(idx_next)) //�����ظ��ĵ�Ԫ���ʱ�������
				break;
			else
				idx_list.push_back(idx_next); //�Ž�ȥ


			bool is_next_dir_exist = true; //��һ�������

			const Tile& tile = map[idx_next.y][idx_next.x];

			if (tile.special_flag == 0)  //���������־��
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

			if (!is_next_dir_exist)   //����ߵ�ͷ�� �¸���Ԫ��û��·����  Ҳ����
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

	//����Ƿ����ظ��ķ���  ��Ҫ�Ƿ�ֹ ·��������ѭ�� �취�Ǽ����һ��Ҫ���ֵ�·���ǲ����Ѿ���������idx_list��
	bool check_duplicate_idx(const SDL_Point& target_idx)
	{
		for (const SDL_Point& idx : idx_list)         // ���д����c++ 11��  for (A : B) �ĺ�����˼�ǣ�����ȡ������ B �е�ÿһ��Ԫ�أ��������� A��Ȼ��ִ��ѭ���塣
		{
			if (idx.x == target_idx.x && idx.y == target_idx.y)  //������ idx �� idx_list ��Ԫ�ص���ʱ���ã����ڱ����б�target_idx �Ǵ����Ŀ��㣬���߶Ա��ж��Ƿ��ظ���
				return true;

		}

		return false;

	}

};




#endif // !_ROUTE_H_

