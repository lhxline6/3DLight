// AnimRotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "AnimRotDlg.h"
#include "afxdialogex.h"
#include "converter.h"
#include "SceneNodeManager.h"
using namespace irr;
using namespace scene;
// CAnimRotDlg 对话框

IMPLEMENT_DYNAMIC(CAnimRotDlg, CDialogEx)

CAnimRotDlg::CAnimRotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnimRotDlg::IDD, pParent)
{

}

CAnimRotDlg::~CAnimRotDlg()
{
}

void CAnimRotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPEED_X, m_speedX);
	DDX_Control(pDX, IDC_SPEED_Y, m_speedY);
	DDX_Control(pDX, IDC_SPEED_Z, m_speedZ);
}


BEGIN_MESSAGE_MAP(CAnimRotDlg, CDialogEx)
	ON_EN_CHANGE(IDC_SPEED_X, &CAnimRotDlg::OnEnChangeSpeedX)
	ON_EN_CHANGE(IDC_SPEED_Y, &CAnimRotDlg::OnEnChangeSpeedY)
	ON_EN_CHANGE(IDC_START_Z, &CAnimRotDlg::OnEnChangeStartZ)
	ON_BN_CLICKED(IDDELETE, &CAnimRotDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CAnimRotDlg 消息处理程序


void CAnimRotDlg::OnEnChangeSpeedX()
{
	CString val;
	m_speedX.GetWindowText(val);
	if(isNumber(val)){
		m_fSpeedX = stof(val);
	}
	else
		m_speedX.SetWindowText(ftos(m_fSpeedX));
}


BOOL CAnimRotDlg::OnInitDialog()
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
	m_fSpeedX = info.rotationSpeed.X;
	m_fSpeedY = info.rotationSpeed.Y;
	m_fSpeedZ = info.rotationSpeed.Z;
	m_speedX.SetWindowText(ftos(m_fSpeedX));
	m_speedY.SetWindowText(ftos(m_fSpeedY));
	m_speedZ.SetWindowText(ftos(m_fSpeedZ));
}


void CAnimRotDlg::OnEnChangeSpeedY()
{
	CString val;
	m_speedY.GetWindowText(val);
	if(isNumber(val)){
		m_fSpeedY = stof(val);
	}
	else
		m_speedY.SetWindowText(ftos(m_fSpeedY));
}


void CAnimRotDlg::OnEnChangeStartZ()
{
	CString val;
	m_speedZ.GetWindowText(val);
	if(isNumber(val)){
		m_fSpeedZ = stof(val);
	}
	else
		m_speedZ.SetWindowText(ftos(m_fSpeedZ));
}


void CAnimRotDlg::OnBnClickedDelete()
{
	EndDialog(IDDELETE);
}
