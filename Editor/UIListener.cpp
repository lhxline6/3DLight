#include "stdafx.h"
#include "UIListener.h"

UIListener * UIListener::instance = NULL;

UIListener * UIListener::getInstance()
{
	if(!instance)
		instance = new UIListener;
	return instance;
}

UIListener::UIListener(void)
{
	m_pFrame = NULL;
}


UIListener::~UIListener(void)
{
}

void UIListener::activate(void * frame)
{
	m_pFrame = frame;
}

void UIListener::update()
{
	if(m_pFrame==NULL)
		return ;
	while(!m_msgQue.empty())
	{
		m_callback[(unsigned int)m_msgQue.front()](m_pFrame);
		m_msgQue.pop();
	}
}

bool UIListener::bind(EventID id,UICallback callback)
{
	if(m_pFrame==NULL)
		return false;
	if(id<0||id>=MSG_NUM)
		return false;
	m_callback[id] = callback;
	return true;
}
void UIListener::sendMessage(EventID id)
{
	m_msgQue.push(id);
}

CWnd * UIListener::getCurrentView()const
{
	return (CWnd*)m_pFrame;
}
