// AnimateDialogCycle.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "AnimateDialogCycle.h"
#include "afxdialogex.h"
#include "SceneNodeManager.h"
#include "converter.h"
using namespace irr;
using namespace scene;
// AnimateDialogCycle 对话框

IMPLEMENT_DYNAMIC(AnimateDialogCycle, CDialogEx)

AnimateDialogCycle::AnimateDialogCycle(CWnd* pParent /*=NULL*/)
	: CDialogEx(AnimateDialogCycle::IDD, pParent)
{

}

AnimateDialogCycle::~AnimateDialogCycle()
{
}

void AnimateDialogCycle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_X, m_startX);
	DDX_Control(pDX, IDC_START_Y, m_startY);
	DDX_Control(pDX, IDC_START_Z, m_startZ);
	DDX_Control(pDX, IDC_RADIUS, m_radius);
	DDX_Control(pDX, IDC_SPEED, m_speed);
}


BEGIN_MESSAGE_MAP(AnimateDialogCycle, CDialogEx)
	ON_EN_CHANGE(IDC_START_X, &AnimateDialogCycle::OnEnChangeStartX)
	ON_EN_CHANGE(IDC_START_Y, &AnimateDialogCycle::OnEnChangeStartY)
	ON_EN_CHANGE(IDC_START_Z, &AnimateDialogCycle::OnEnChangeStartZ)
	ON_EN_CHANGE(IDC_RADIUS, &AnimateDialogCycle::OnEnChangeRadius)
	ON_EN_CHANGE(IDC_SPEED, &AnimateDialogCycle::OnEnChangeSpeed)
	ON_BN_CLICKED(IDDELETE, &AnimateDialogCycle::OnBnClickedDelete)
	ON_COMMAND(ID_ADD_CONE, &AnimateDialogCycle::OnAddCone)
	ON_COMMAND(ID_ADD_CONE_POP, &AnimateDialogCycle::OnAddConePop)
	ON_COMMAND(ID_ADD_BILLBOARD, &AnimateDialogCycle::OnAddBillboard)
END_MESSAGE_MAP()


// AnimateDialogCycle 消息处理程序


BOOL AnimateDialogCycle::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr==NULL)
		return false;
	ISceneNode * node = snmgr->getCurrentNode();
	AnimateInfo info;
	if(node!=NULL)
		info = snmgr->getAnimatedValue();
	CString buf;
	m_fStartX = info.center.X;
	m_fStartY = info.center.Y;
	m_fStartZ = info.center.Z;
	m_fSpeed = info.speed;
	m_fRadius = info.radius;
	m_startX.SetWindowText(ftos(m_fStartX));
	m_startY.SetWindowText(ftos(m_fStartY));
	m_startZ.SetWindowText(ftos(m_fStartZ));
	m_speed.SetWindowText(ftos(m_fSpeed));
	m_radius.SetWindowText(ftos(m_fRadius));
}



void AnimateDialogCycle::OnEnChangeStartX()
{
	CString val;
	m_startX.GetWindowText(val);
	if(isNumber(val)){
		m_fStartX = stof(val);
	}
	else
		m_startX.SetWindowText(ftos(m_fStartX));
}


void AnimateDialogCycle::OnEnChangeStartY()
{
	CString val;
	m_startY.GetWindowText(val);
	if(isNumber(val)){
		m_fStartY = stof(val);
	}
	else
		m_startY.SetWindowText(ftos(m_fStartY));
}


void AnimateDialogCycle::OnEnChangeStartZ()
{
	CString val;
	m_startZ.GetWindowText(val);
	if(isNumber(val)){
		m_fStartZ = stof(val);
	}
	else
		m_startZ.SetWindowText(ftos(m_fStartZ));
}


void AnimateDialogCycle::OnEnChangeRadius()
{
	CString val;
	m_radius.GetWindowText(val);
	if(isNumber(val)){
		m_fRadius = stof(val);
	}
	else
		m_radius.SetWindowText(ftos(m_fRadius));
}


void AnimateDialogCycle::OnEnChangeSpeed()
{
	CString val;
	m_speed.GetWindowText(val);
	if(isNumber(val)){
		m_fSpeed = stof(val);
	}
	else
		m_speed.SetWindowText(ftos(m_fSpeed));
}


void AnimateDialogCycle::OnBnClickedDelete()
{
	EndDialog(IDDELETE);
}


void AnimateDialogCycle::OnAddCone()
{
	// TODO: 在此添加命令处理程序代码
}


void AnimateDialogCycle::OnAddConePop()
{
	// TODO: 在此添加命令处理程序代码
}


void AnimateDialogCycle::OnAddBillboard()
{
	// TODO: 在此添加命令处理程序代码
}
