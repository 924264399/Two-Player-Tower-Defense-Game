#ifndef _MAP_H_
#define _MAP_H_

#include "tile.h"
#include "route.h"

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map> //存储离散数据合适 哈希表


class Map
{

public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;




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

		while (std::getline(file, str_line))          /////// 外层循环读文件的每一行（用getline默认分隔符'\n'） 
		{

			str_line = trim_str(str_line);  //去除前后空白符

			if (str_line.empty())            //如果有一行是空的 就跳过
				continue;


			idx_x = -1, idx_y++;
			tile_map_temp.emplace_back(); //二维数组是这样添加的

			std::string str_tile;
			std::stringstream str_stream(str_tile);

			while (std::getline(str_stream, str_tile, ','))   /////内层循环处理每一列
			{
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile); //核心

			}

		}
		///////////////依0,1,2
		/////////////// 3, 4, 5 数据为例
		///////////////外层循环会直接读到 0 1 2
		///////////////内层循环因为getline有，的分隔符  所以依次读0 1 2 


		file.close();


		if (tile_map_temp.empty() || tile_map_temp[0].empty())
			return false;
		
		tile_map = tile_map_temp;

		generate_map_cache();

		return true;

	}


	///地图宽度  加const是这个方法对内不会修改
	size_t get_width() const
	{

		if (tile_map.empty())
			return 0;

		return tile_map[0].size();
	}


	size_t get_height() const
	{

		return tile_map.size();

	}

	const TileMap& get_tile_map() const   //返回引用 前面const修饰 只读不能改   后面const 确保函数内部不改
	{

		return tile_map;
	}


	const SDL_Point& get_idx_home() const
	{

		return idx_home;
	}

	const SpawnerRoutePool& get_idx_spawner_pool() const
	{

		return spawner_route_pool;

	}

	void place_towewr(const SDL_Point& idx_tile)
	{
		tile_map[idx_tile.y][idx_tile.x].has_tower = true;

	}



private:

	TileMap tile_map;  //动态二维数组
	SDL_Point idx_home = { 0 };
	SpawnerRoutePool spawner_route_pool;


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



	// 拿csv地图数据 到 tile类的属性
	//具体就是从str字符串 解析到  tile对象
	void load_tile_from_string(Tile& tile, const std::string& str)
	{

		std::string str_tidy = trim_str(str);

		std::string str_value;
		std::vector<int> values;

		std::stringstream str_stream(str_tidy);


		while (std::getline(str_stream, str_value, '\\'))
		{
			int value;

			try
			{
				value = std::stoi(str_value);
			}

			catch (const std::exception)
			{

				value = -1;

			}
			values.push_back(value);

		}

		//这是tile类的属性 在csv文件里  每个格子有存储4个值  分别代表基础地块 - 装饰卡片（树木啊 牌子啊）-行进路线（箭头）-特殊标志（刷怪点 家）  这里需要分别给到相应的属性
		//之所以下面这么写 主要还是为了防止数据有问题
		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];    // 条件 ？ 是的结果 ： 否的结果
		tile.decoration = (values.size() < 2) ? -1 : values[1];

		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2] );

		tile.special_flag = (values.size() <= 3) ? -1 : values[3];

	}


	//生成地图缓存  遍历每个单元格
	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];

				if (tile.special_flag < 0)
					continue;


				if (tile.special_flag == 0) // 如果是家
				{
					idx_home.x = x;
					idx_home.y = y;
				}
				else                        //如果是刷怪点  那么就生成这个路径信息 存储到spawner_route_pool这个unordered_map数据格式内
				{
					spawner_route_pool[tile.special_flag] = Route(tile_map, { x,y }); //这里可能反了

				}


			}
		}

	}

};



#endif // !_MAP_H_

