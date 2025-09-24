#ifndef  _MANAGER_H_
#define _MANAGER_H

template<typename T>

//c++ 经典的单例模式
//何为单例？ 单例模式确保一个类​​只有一个实例​​，并提供一个​​全局访问点​​。这在需要全局唯一对象时非常有用（如配置管理器、日志系统等）
class Manager
{

public:

	// 静态方法获取唯一实例  相当于在外部直接调用这个静态方法来创建实例
	static T* instance()
	{
		if (!manager1)
			manager1 = new T();

		return manager1;

	}

	//析构
	

private:
	static T* manager1; // 静态成员指针（存储唯一实例）这里我为了区分给这个实例取名是manager1


//改为protected 允许子类访问父类构造函数和析构函数
protected:

	//构造函数私有 无法创建实例化对象（改为protected后 依旧）
	Manager() = default; 


	~Manager() = default;


	//拷贝构造函数 要删除 运算符也要删除
	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;

};


//模块相关的代码
template<typename T>
T* Manager<T> ::manager1 = nullptr;


#endif // ! _MANAGER_H_
