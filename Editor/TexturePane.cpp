// TexturePane.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "TexturePane.h"
#include "MainFrm.h"

// CTexturePane

IMPLEMENT_DYNAMIC(CTexturePane, CDockablePane)

CTexturePane::CTexturePane()
{
}

CTexturePane::~CTexturePane()
{
}

BOOL CTexturePane::create(CFrameWndEx * parent)
{
	if(!this->Create(CString("纹理"),parent,CRect(0,0, 600, 180),TRUE,NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		CBRS_BOTTOM))
	{
		TRACE0("未能创建纹理窗口");
		return FALSE;
	}
	this->EnableDocking(CBRS_ALIGN_ANY);
	parent->DockPane(this);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CTexturePane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CLOSE()
//	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTexturePane 消息处理程序

void CTexturePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect client;
	GetClientRect(&client);
	dc.FillRect(&client,&CBrush(RGB(245,245,245)));
}


int CTexturePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(m_wndTextureList.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,600,150),this,1)==false)
	{
		TRACE0("未能创建纹理列表");
		return -1;
	}
	return 0;
}


void CTexturePane::OnClose()
{
	CDockablePane::OnClose();
}

