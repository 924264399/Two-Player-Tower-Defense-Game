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

		if (!file.good()) return false;  //读文件基础流

		TileMap tile_map_temp;

		int idx_x = -1, idx_y = -1;

		std::string str_line;

		while (std::getline(file, str_line))          // 循环读文件的每一行（用getline默认分隔符'\n'）
		{



		}



	}

private:

	TileMap tile_map;  //动态二维数组
	SDL_Point idx_home = { 0 };


private:

	//c++ 一个标准的截取首位空白符号的函数
	std::string trim_str(const std::string& str)
	{
		//" 6\57\4\-1 "
		// "6\57\4\-1"  把首尾字符串进行截取
		size_t begin_idx = str.find_first_not_of(" \t");   //1.用 find_first_not_of(" \t") 找到第一个有效字符的位置索引 begin_idx（跳过开头的空格 / 制表符）
	
		if (begin_idx == std::string::npos)                //	如果没找到（全是空格），返回空字符串。
			return "";

		size_t end_idx = str.find_last_not_of(" \t");     //用 find_last_not_of(" \t") 找到最后一个有效字符的位置 end_idx（跳过结尾的空格 / 制表符）；
		size_t idx_range = end_idx - begin_idx + 1;       // 计算需要截取的长度：end_idx - begin_idx + 1（+1 是因为包含首尾两个位置）；

		return str.substr(begin_idx, idx_range);          //从 begin_idx 开始，截取对应长度的子字符串，就是结果。

	}



};



#endif // !_MAP_H_

