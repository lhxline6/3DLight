#pragma once

#include<queue>

/*======================
UIListener:
	为引擎内核提供一个独立于输出窗口的
	消息传递通道。其中的窗口一般是引擎
	使用的View，在该View的初始化函数
	中使用activate注册窗口，在Update
	方法中调用update即可激活消息循环
======================*/
typedef void (*UICallback)(void*);
class UIListener
{
public:
	/*=============================
	*回调函数存放在数组当中，每个信息对应一个整数
	*下标，这个下标处存储着对应的回调函数
	*不提供从外部添加信息的接口。
	*要添加一个信息：
	*1.设置对应的EventID，必须是连续的
	*2.将MSG_NUM改为当前EventID中项目的数量
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
	/*消息队列*/
	std::queue<EventID> m_msgQue;
	/*回调函数数组*/ 
	UICallback m_callback[MSG_NUM];
	/*当前领域中接收消息的窗口指针*/
	void * m_pFrame;
public:
	/*使用对象前一定要用activate绑定一个窗口，否则所有函数都不会生效*/
	void activate(void * frame);
	/*在逐帧刷新过程中调用update以激活消息*/
	void update();
	/*bind把一个回调函数绑定在相应的消息上，如果不是系统内定消息，则无效*/
	bool bind(EventID id,UICallback callback);
	/*添加一个系统消息到消息队列*/
	void sendMessage(EventID id);
	/*提供一个接口来获取当前view信息*/
	CWnd * getCurrentView()const;
};

