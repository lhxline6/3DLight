#include "stdafx.h"
#include "PropertyPane.h"
#include "UIListener.h"
#include "SceneNodeManager.h"
#include "Editor.h"
#include "EditorView.h"
#include <sstream>
#include "converter.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace std;


CPropertyPane::CPropertyPane(void)
{
}


CPropertyPane::~CPropertyPane(void)
{
}

void CPropertyPane::UpdatePropList()
{
	
	m_wndPropList.RemoveAll();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	
	m_wndPropList.idPropList.clear();
	//读取节点信息
	NodeInfo node;

	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr==NULL)
			return ;
	node = snmgr->getCurrentNodeInfo();

	//添加基本信息
	CMFCPropertyGridProperty * pProp;
	IdProperty  pIdProp;
	CMFCPropertyGridProperty * pId = new CMFCPropertyGridProperty(_T("基本信息"));
	CString str_type;
	
	switch(node.nodeType)
	{
	case TYPE_CUBE:
		str_type = "正方体";
		break;
	case TYPE_SPHERE:
		str_type = "球体";
		break;
	case TYPE_ROOT:
		str_type = "根节点";
		break;
	case TYPE_LIGHT:
		str_type = "光源";
		break;
	case TYPE_CYLINDER:
		str_type = "圆柱体";
		break;
	case TYPE_CONE:
		str_type = "圆锥体";
		break;
	case TYPE_BILLTEXT:
		str_type = "公告板";
		break;
	case TYPE_PARTICLE:
		str_type = "粒子";
		break;
	default:
		str_type = "";
	}


	pProp = new CMFCPropertyGridProperty(_T("类型"),str_type);
	pProp->AllowEdit(false);
	pId->AddSubItem(pProp);
	
	CString str_name = string2CString(node.name);
	CMFCPropertyGridProperty * pName = new CMFCPropertyGridProperty(_T("名称"),str_name);
	pId->AddSubItem(pName);
	
	pIdProp.pId = NAME;
	pIdProp.pProp = pName;
	m_wndPropList.idPropList.push_back(pIdProp);
	
	m_wndPropList.AddProperty(pId);
	//添加几何体属性
	if(node.nodeType==scene::TYPE_CUBE ||
		node.nodeType==scene::TYPE_SPHERE||
		node.nodeType==scene::TYPE_CONE||
		node.nodeType==scene::TYPE_CYLINDER||
		node.nodeType == scene::TYPE_BILLTEXT||
		node.nodeType == scene::TYPE_PARTICLE)
	{
		
		CMFCPropertyGridProperty * pGroup = new CMFCPropertyGridProperty(_T("几何属性"),0,false);

		//以下注释是几何体光照属性编辑，添加环境光后，暂时禁用
		/*
		CString cstr_option = node.enableLight?CString("允许"):CString("不允许");
		CMFCPropertyGridProperty * pEnLight = new CMFCPropertyGridProperty(_T("允许光照"),cstr_option,_T(""));
		pEnLight->AddOption(L"允许");
		pEnLight->AddOption(L"不允许");
		pIdProp.pId = E_LIGHT;
		pIdProp.pProp = pEnLight;
		m_wndPropList.idPropList.push_back(pIdProp);

		pGroup->AddSubItem(pEnLight);
		*/

		CMFCPropertyGridProperty * pPos = new CMFCPropertyGridProperty(_T("位置"),0,false);
		pProp = new CMFCPropertyGridProperty(_T("X"),ftos(node.position.X));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_X;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Y"),ftos(node.position.Y));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_Y;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Z"),ftos(node.position.Z));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_Z;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		CMFCPropertyGridProperty * pSize = new CMFCPropertyGridProperty(_T("尺寸"),0,false);
		pProp = new CMFCPropertyGridProperty(_T("X"),ftos(node.size.X));
		pProp->AllowEdit(true);
		pSize->AddSubItem(pProp);
		pIdProp.pId = S_X;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		

		pProp = new CMFCPropertyGridProperty(_T("Y"),ftos(node.size.Y));
		pProp->AllowEdit(true);
		pSize->AddSubItem(pProp);
		pIdProp.pId = S_Y;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Z"),ftos(node.size.Z));
		pProp->AllowEdit(true);
		pSize->AddSubItem(pProp);
		pIdProp.pId = S_Z;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		
		CMFCPropertyGridProperty * pRot = new CMFCPropertyGridProperty(_T("旋转"),0,false);
		pProp = new CMFCPropertyGridProperty(_T("X"),ftos(node.rotate.X));
		pProp->AllowEdit(true);
		pRot->AddSubItem(pProp);
		pIdProp.pId = R_X;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		

		pProp = new CMFCPropertyGridProperty(_T("Y"),ftos(node.rotate.Y));
		pProp->AllowEdit(true);
		pRot->AddSubItem(pProp);
		pIdProp.pId = R_Y;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Z"),ftos(node.rotate.Z));
		pProp->AllowEdit(true);
		pRot->AddSubItem(pProp);
		pIdProp.pId = R_Z;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pGroup->AddSubItem(pPos);
		pGroup->AddSubItem(pSize);
		pGroup->AddSubItem(pRot);
		m_wndPropList.AddProperty(pGroup);
	}
	else if(node.nodeType==scene::TYPE_LIGHT)
	{
		CMFCPropertyGridProperty * pGroup = new CMFCPropertyGridProperty(_T("光照属性"),0,false);
		CMFCPropertyGridProperty * pPos = new CMFCPropertyGridProperty(_T("位置"),0,false);
		pProp = new CMFCPropertyGridProperty(_T("X"),ftos(node.position.X));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_X;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Y"),ftos(node.position.Y));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_Y;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Z"),ftos(node.position.Z));
		pProp->AllowEdit(true);
		pPos->AddSubItem(pProp);
		pIdProp.pId = P_Z;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		CMFCPropertyGridProperty * pColor = new CMFCPropertyGridProperty(_T("颜色"),0,false);
		pProp = new CMFCPropertyGridProperty(_T("Alpha"),ftos(node.color.a));
		pProp->AllowEdit(true);
		pColor->AddSubItem(pProp);
		pIdProp.pId = C_A;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Red"),ftos(node.color.r));
		pProp->AllowEdit(true);
		pColor->AddSubItem(pProp);
		pIdProp.pId = C_R;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		

		pProp = new CMFCPropertyGridProperty(_T("Green"),ftos(node.color.g));
		pProp->AllowEdit(true);
		pColor->AddSubItem(pProp);
		pIdProp.pId = C_G;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);

		pProp = new CMFCPropertyGridProperty(_T("Blue"),ftos(node.color.b));
		pProp->AllowEdit(true);
		pColor->AddSubItem(pProp);
		pIdProp.pId = C_B;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		
		pProp = new CMFCPropertyGridProperty(_T("半径"),ftos(node.radius));
		pProp->AllowEdit(true);
		pIdProp.pId = RADIUS;
		pIdProp.pProp = pProp;
		m_wndPropList.idPropList.push_back(pIdProp);
		

		pGroup->AddSubItem(pPos);
		pGroup->AddSubItem(pColor);
		pGroup->AddSubItem(pProp);
		m_wndPropList.AddProperty(pGroup);
	}
}

BOOL CPropertyPane::create(CFrameWndEx * parent)
{
	if(!this->Create(CString("属性"), parent, CRect(0, 0, 200, 330), 
		TRUE, NULL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		CBRS_LEFT))
	{
		TRACE0("未能创建属性窗口\n");
		return false;
	}
	this->EnableDocking(CBRS_ALIGN_ANY);
	parent->DockPane(this);
	return true;
}

BEGIN_MESSAGE_MAP(CPropertyPane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CPropertyPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CDockablePane::OnPaint()
	RECT client;
	GetClientRect(&client);
	dc.FillRect(&client,&CBrush(RGB(245,245,245)));
}

int CPropertyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	RECT client;
	GetClientRect(&client);
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD,client, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}
	return 0;
}

void CPaneCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str = pProp->FormatProperty();
	IdProperty prop;
	prop.pProp = NULL;
	//寻找当前被改变的节点
	for(UINT i=0;i<idPropList.size();i++)
	{
		if(pProp==idPropList[i].pProp)
		{
			prop = idPropList[i];
			break;
		}
	}
	if(prop.pProp==NULL)
		return;
	
	//判断当前节点的性质
	float res;
	bool b_res;
	std::string str_res;
	if(prop.pId==NAME)
	{
		str_res = CString2string(str);
	}
	else if(prop.pId==E_LIGHT)
	{
		if(str==CString("允许")) 
			b_res = true;
		else
			b_res = false;
	}
	else
	{
		if(isNumber(str))
			res = stof(str);
		else return;
	}
	NodeInfo node;
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(!snmgr)
		return;
	node = snmgr->getCurrentNodeInfo();
	switch(prop.pId)
	{
	case P_X:
		node.position.X = res;
		break;
	case P_Y:
		node.position.Y = res;
		break;
	case P_Z:
		node.position.Z = res;
		break;
	case S_X:
		node.size.X = res;
		break;
	case S_Y:
		node.size.Y = res;
		break;
	case S_Z:
		node.size.Z = res;
		break;
	case R_X:
		node.rotate.X = res;
		break;
	case R_Y:
		node.rotate.Y = res;
		break;
	case R_Z:
		node.rotate.Z = res;
		break;
	case C_A:
		node.color.a = res;
		break;
	case C_R:
		node.color.r = res;
		break;
	case C_G:
		node.color.g = res;
		break;
	case C_B:
		node.color.b = res;
		break;
	case E_LIGHT:
		node.enableLight = b_res;
		break;
	case RADIUS:
		node.radius = res;
		break;
	case NAME:
		node.name = str_res;
		break;
	default:break;
	}

	snmgr->setCurrentValue(node);
	UIListener * ul = UIListener::getInstance();
	ul->sendMessage(UIListener::NodeTreePane);
	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

