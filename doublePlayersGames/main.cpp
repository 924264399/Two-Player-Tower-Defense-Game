#include <iostream>

#include "manager.h"




Manager* Manager::manager1 = nullptr;//C++ 要求静态成员必须在类外进行​​定义(赋值啥的）​

int main()
{
	Manager *manager1 = Manager::instance(); //instance()是静态成员函数哦

		
	std::cout << "hellow world" << std::endl;

	return 0;




}