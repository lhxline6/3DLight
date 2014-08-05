// TextureList.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "TextureList.h"
#include "GdiplusInterface.h"
#include "converter.h"
#include "BitmapManager.h"
#include "SceneNodeManager.h"
#include "FileManager.h"
using namespace Gdiplus;
using namespace std;
using namespace irr;
// CTextureList

IMPLEMENT_DYNAMIC(CTextureList, CWnd)

CTextureList::CTextureList()
{
	m_iCurSel = -1;
	m_iElemWidth = 120;
	m_iElemHeight = 120;
}

CTextureList::~CTextureList()
{
}

void CTextureList::SetCurSel(int id)
{
	if(id<0||id>=m_storage.size())
		id = -1;
	m_iCurSel = id;
	Invalidate(false);
}

int CTextureList::GetCurSel()const
{
	return m_iCurSel;
}

void CTextureList::drawElement(const Node & node)
{
	BitmapManager * bmgr = BitmapManager::GetInstance();
	Graphics graph(node.m_pDC->GetSafeHdc());
	Bitmap * bitmap = bmgr->GetBitmap(node.m_strFilePath);
	if(bitmap&&bitmap->GetFrameDimensionsCount()>0)
	{
		graph.DrawImage(bitmap,CRect2Rect(node.m_rectElement),
		0,0,bitmap->GetWidth(),bitmap->GetHeight(),UnitPixel);
	}
	if(node.m_id==m_iCurSel)
	{
		graph.FillRectangle(&SolidBrush(Color(120,30,30,240)),
			CRect2Rect(node.m_rectElement));
	}
}
int CTextureList::updateSel(const CPoint & point)
{
	for(int i=0;i<m_storage.size();i++)
	{
		if(m_storage[i].m_rectElement.PtInRect(point))
			return i;
	}
	return -1;
}

bool CTextureList::AddItem(Node node)
{
	//项目是否已存在？若存在则不插入直接返回true
	for(int i=0;i<m_storage.size();i++)
	{
		if(m_storage[i].m_strFilePath==node.m_strFilePath)
			return true;
	}
	//验证插入项的合法性
	BitmapManager * bmgr = BitmapManager::GetInstance();
	bmgr->AddBitmap(node.m_strFilePath);
	Bitmap * pBitmap = bmgr->GetBitmap(node.m_strFilePath);
	if(pBitmap==NULL)
		return false;
	node.m_id = m_storage.size();
	int id = node.m_id;
	node.m_rectElement = CRect(m_iElemWidth*id,10,m_iElemHeight*(id+1),10+m_iElemHeight);
	m_storage.push_back(node);
	Resize();
	return true;
}
void CTextureList::RemoveItem(int id)
{
	if(id<0||id>=m_storage.size())
		return ;
	BitmapManager * bmgr = BitmapManager::GetInstance();
	bmgr->DeleteBitmap(m_storage[id].m_strFilePath);
	for(int i=id;i<m_storage.size()-1;i++)
		m_storage[i] = m_storage[i+1];
	m_storage.pop_back();
	Resize();
}

void CTextureList::Resize()
{
	CRect client;
	GetClientRect(&client);
	int ele_num = client.Width()/m_iElemWidth;
	int scroll_num = m_storage.size() - ele_num;
	if(scroll_num<=0)
		m_scroll.ShowWindow(SW_HIDE);
	else
	{
		m_scroll.ShowWindow(SW_SHOW);
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE|SIF_RANGE;
		info.nMin = 0;
		info.nMax = scroll_num;
		info.nPage = 1;
		m_scroll.SetScrollInfo(&info,true);
	}
	Invalidate(false);
}

BEGIN_MESSAGE_MAP(CTextureList, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CTextureList 消息处理程序

void CTextureList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect client;
	GetClientRect(&client);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,client.Width(),client.Height());
	dcMem.SelectObject(&bitmap);

	int aligns = m_scroll.GetScrollPos();
	dcMem.FillRect(&client,&CBrush(RGB(245,245,245)));
	for(int i=0;i<m_storage.size();i++)
	{
		m_storage[i].m_id = i;
		m_storage[i].m_pDC = &dcMem;
		m_storage[i].m_rectElement = 
			CRect(m_iElemWidth*(i-aligns),
			10,
			m_iElemHeight*(i+1-aligns),
			10+m_iElemHeight);
		drawElement(m_storage[i]);
	}

	dc.BitBlt(0,0,client.Width(),client.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bitmap.DeleteObject();
}



void CTextureList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonDown(nFlags, point);
	int id = updateSel(point);
	SetCurSel(id);
}


int CTextureList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect client;
	GetClientRect(&client);
	m_scroll.Create(SBS_HORZ|SBS_BOTTOMALIGN|WS_CHILD|WS_VISIBLE,
		CRect(client.left,client.bottom-20,client.right,client.bottom),
		this, 1);
	AddItem(Node(L"media/detailmap3.jpg"));
	AddItem(Node(L"media/rockwall.jpg"));
	AddItem(Node(L"media/rockwall_height.bmp"));
	AddItem(Node(L"media/stones.jpg"));
	AddItem(Node(L"media/wall.jpg"));
	AddItem(Node(L"media/wall.bmp"));
	AddItem(Node(L"media/water.jpg"));
	return 0;
}


void CTextureList::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Resize();
	// TODO: 在此处添加消息处理程序代码
}


void CTextureList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	pScrollBar->GetScrollInfo(&si, SIF_POS|SIF_PAGE|SIF_RANGE);
	int nHscroll = si.nPos;  

	switch(nSBCode)  
	{  
	case SB_LINERIGHT:
		nHscroll += 1;
		if (nHscroll > (si.nMax - si.nMin))  
		{  
			nHscroll = si.nMax - si.nMin;  
		}  
		break;  
  
	case SB_LINELEFT:
		nHscroll -= 1;  
		if (nHscroll < si.nMin)  
		{  
			nHscroll = 0;  
		}  
		break;  
	case SB_PAGERIGHT:  
		nHscroll += si.nPage;   
		if (nHscroll > (si.nMax - si.nMin))  
		{  
			nHscroll = si.nMax - si.nMin;  
		}  
		break;  
	case SB_PAGELEFT:  
		nHscroll -= si.nPage;  
		if (nHscroll < si.nMin)  
		{  
			nHscroll = 0;  
		}  
		break;  
	case  SB_THUMBTRACK: 
		nHscroll = nPos;
		break;  
	}
	si.fMask = SIF_POS;  
    si.nPos = nHscroll;  
    pScrollBar->SetScrollInfo(&si, TRUE); 
	Invalidate(false);
}

void CTextureList::Apply()
{
	if(m_iCurSel==-1)
		return ;
	scene::ISceneNodeManager * snmgr = 
		scene::ISceneNodeManager::getInstance();	
	if(snmgr==NULL)
		return;
	scene::ISceneNode * node = snmgr->getCurrentNode();
	if(node==NULL)
		return ;
	//读取节点材质
	IrrlichtDevice * device = snmgr->getDevice();
	video::IVideoDriver * driver = device->getVideoDriver();
	CString cstr_path = m_storage[m_iCurSel].m_strFilePath;
	string str_path = CString2string(cstr_path);
	video::ITexture * texture = driver->getTexture(str_path.c_str());
	node->setMaterialTexture(0,texture);

	//modified
	snmgr->setTexture(node->getID(),str_path);
	//
}


void CTextureList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Apply();
	CWnd::OnLButtonDblClk(nFlags, point);
}
