#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timmer
{
public:
	Timmer() =default;
	~Timmer() = default;

private:

	double pass_time = 0.0; //�ӿ�ʼ������ȥ�˶���ʱ��
	double wait_time = 0.0; // Ҫ�ȴ���ʱ�� �����˶���ʱ��������ʱ���Ĺ���

	bool pasued = false; //��ͣ

	bool shotted = false; //��ʱ����shotted  ������һ��  �����ǵ��ʴ���  Ҳ�����Ƕ�δ���  ����ǵ��εĻ� ����Ҫһ����������¼���Ѿ���������

	bool one_shot = false; //�Ƿ���Ҫ���δ���







};




#endif

