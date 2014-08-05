#include "stdafx.h"
#include "NodeTreePane.h"
#include "SceneNodeManager.h"
#include "UIListener.h"
#include "converter.h"
#include "EditorView.h"
#include "Editor.h"
#include "EditCamera.h"
#include "MainFrm.h"
#include<string>
using namespace irr;
using namespace scene;
using namespace std;

//============CNodeTreePane==========
CNodeTreePane::CNodeTreePane(void)
{

}


CNodeTreePane::~CNodeTreePane(void)
{
}

BOOL CNodeTreePane::create(CFrameWndEx * parent)
{
	if(!this->Create(CString("节点"),parent,CRect(0, 300, 200, 600),TRUE,NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		CBRS_RIGHT))
	{
		TRACE0("未能创建节点窗口");
		return FALSE;
	}
	this->EnableDocking(CBRS_ALIGN_ANY);
	parent->DockPane(this);
	return TRUE;
}

void CNodeTreePane::UpdateTreeNode()
{
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr==NULL)
		return;
	m_wndNodeTree.DeleteAllItems();
	m_storage.clear();

	vector<IMeshSceneNode *> vect_node = snmgr->getMeshSceneNodes();
	vector<ILightSceneNode *> vect_light = snmgr->getLightSceneNodes();
	//添加树内容
	HTREEITEM hRoot = m_wndNodeTree.InsertItem(CString("几何体"),0,0);
	m_wndNodeTree.SetItemState(hRoot,TVIS_BOLD,TVIS_BOLD);
	for(int i=0;i<vect_node.size();i++)
	{
		IMeshSceneNode * p_node = vect_node[i];
		CString cstr_name = string2CString(std::string(p_node->getName()));
		HTREEITEM hNode = m_wndNodeTree.InsertItem(cstr_name,0,0,hRoot);
		m_storage.push_back(Node(hNode,p_node));
		m_wndNodeTree.SetItemState(hNode,TVIS_BOLD,TVIS_BOLD);
	}	
	
	HTREEITEM hLight = m_wndNodeTree.InsertItem(CString("光照"),0,0);
	m_wndNodeTree.SetItemState(hLight,TVIS_BOLD,TVIS_BOLD);
	for(int i=0;i<vect_light.size();i++)
	{
		ILightSceneNode * p_light = vect_light[i];
		CString cstr_name = string2CString(std::string(p_light->getName()));
		HTREEITEM hNode = m_wndNodeTree.InsertItem(cstr_name,0,0,hLight);
		m_storage.push_back(Node(hNode,p_light));
		m_wndNodeTree.SetItemState(hNode,TVIS_BOLD,TVIS_BOLD);
	}

	m_wndNodeTree.Expand(hRoot,TVE_EXPAND);
	m_wndNodeTree.Expand(hLight,TVE_EXPAND);
}
void CNodeTreePane::OnSelChange()
{
	HTREEITEM hItem = m_wndNodeTree.GetSelectedItem();
	Node node;
	int i;
	for(i=0;i<m_storage.size();i++)
	{
		if(hItem==m_storage[i].hItem)
		{
			node = m_storage[i];
			break;
		}
	}
	if(i==m_storage.size())
		return;
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr)
	{
		snmgr->setCurrentNode(node.pSceneNode);
		//将摄像机转向当前节点
		CEditorApp * app = (CEditorApp*)::AfxGetApp();
		CEditorView * view = app->GetCurrentView();
		scene::ICameraSceneNode * camera = 
			view->device->getSceneManager()->getActiveCamera();
		if(camera)
		{
			irr::scene::setEditCameraTarget(camera,node.pSceneNode);
		}	
	}
	
}

BEGIN_MESSAGE_MAP(CNodeTreePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


int CNodeTreePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	RECT client;
	GetClientRect(&client);
	if (!m_wndNodeTree.Create(WS_VISIBLE | WS_CHILD,client, this, 2))
	{
		TRACE0("未能创建节点树窗格\n");
		return -1;      // 未能创建
	}
	m_wndNodeTree.BindParent(this);
	UpdateTreeNode();
	return 0;
}


void CNodeTreePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect client;
	GetClientRect(&client);
	dc.FillRect(&client,&CBrush(RGB(245,245,245)));
}


void CNodeTreePane::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDockablePane::OnLButtonDblClk(nFlags, point);
}
