#ifndef _MAP_H_
#define _MAP_H_

#include "tile.h"

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>

class Map
{
public:
	Map() = default;
	~Map() = default;

	bool load(const std::string& path)
	{
		std::ifstream file(path);

		if (!file.good()) return false;  //���ļ�������

		TileMap tile_map_temp;

		int idx_x = -1, idx_y = -1;

		std::string str_line;

		while (std::getline(file, str_line))          // ѭ�����ļ���ÿһ�У���getlineĬ�Ϸָ���'\n'��
		{



		}



	}

private:

	TileMap tile_map;  //��̬��ά����
	SDL_Point idx_home = { 0 };


private:

	//c++ һ����׼�Ľ�ȡ��λ�հ׷��ŵĺ���
	std::string trim_str(const std::string& str)
	{
		//" 6\57\4\-1 "
		// "6\57\4\-1"  ����β�ַ������н�ȡ
		size_t begin_idx = str.find_first_not_of(" \t");   //1.�� find_first_not_of(" \t") �ҵ���һ����Ч�ַ���λ������ begin_idx��������ͷ�Ŀո� / �Ʊ����
	
		if (begin_idx == std::string::npos)                //	���û�ҵ���ȫ�ǿո񣩣����ؿ��ַ�����
			return "";

		size_t end_idx = str.find_last_not_of(" \t");     //�� find_last_not_of(" \t") �ҵ����һ����Ч�ַ���λ�� end_idx��������β�Ŀո� / �Ʊ������
		size_t idx_range = end_idx - begin_idx + 1;       // ������Ҫ��ȡ�ĳ��ȣ�end_idx - begin_idx + 1��+1 ����Ϊ������β����λ�ã���

		return str.substr(begin_idx, idx_range);          //�� begin_idx ��ʼ����ȡ��Ӧ���ȵ����ַ��������ǽ����

	}



};



#endif // !_MAP_H_

