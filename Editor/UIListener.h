#pragma once

#include<queue>

/*======================
UIListener:
	Ϊ�����ں��ṩһ��������������ڵ�
	��Ϣ����ͨ�������еĴ���һ��������
	ʹ�õ�View���ڸ�View�ĳ�ʼ������
	��ʹ��activateע�ᴰ�ڣ���Update
	�����е���update���ɼ�����Ϣѭ��
======================*/
typedef void (*UICallback)(void*);
class UIListener
{
public:
	/*=============================
	*�ص�������������鵱�У�ÿ����Ϣ��Ӧһ������
	*�±꣬����±괦�洢�Ŷ�Ӧ�Ļص�����
	*���ṩ���ⲿ�����Ϣ�Ľӿڡ�
	*Ҫ���һ����Ϣ��
	*1.���ö�Ӧ��EventID��������������
	*2.��MSG_NUM��Ϊ��ǰEventID����Ŀ������
	*============================*/
	static const int MSG_NUM = 3;
	enum EventID
	{
		PropertyPane = 0,
		NodeTreePane = 1,
		InitialScene = 2
	};
private:
	UIListener(void);
	static UIListener * instance;
public:
	~UIListener(void);
	static UIListener * getInstance();

private:
	/*��Ϣ����*/
	std::queue<EventID> m_msgQue;
	/*�ص���������*/ 
	UICallback m_callback[MSG_NUM];
	/*��ǰ�����н�����Ϣ�Ĵ���ָ��*/
	void * m_pFrame;
public:
	/*ʹ�ö���ǰһ��Ҫ��activate��һ�����ڣ��������к�����������Ч*/
	void activate(void * frame);
	/*����֡ˢ�¹����е���update�Լ�����Ϣ*/
	void update();
	/*bind��һ���ص�����������Ӧ����Ϣ�ϣ��������ϵͳ�ڶ���Ϣ������Ч*/
	bool bind(EventID id,UICallback callback);
	/*���һ��ϵͳ��Ϣ����Ϣ����*/
	void sendMessage(EventID id);
	/*�ṩһ���ӿ�����ȡ��ǰview��Ϣ*/
	CWnd * getCurrentView()const;
};

