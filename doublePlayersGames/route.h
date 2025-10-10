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

		SDL_Point idx_next = idx_origin;  //SDL_point ֻ��������Ա��x������λ�ã��� y������λ�ã��������������ͣ���Ӧ��Ļ�������ϵ��������ꡣ


		while (true)
		{
			//�������ֱ �� ˮƽ������ ������ ��ͼ�߽�
			if (idx_next.x >= width_map || idx_next.y >= height_map)
				break;

			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next); //�Ž�ȥ



	

		}



	}

	~Route()
	{



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

