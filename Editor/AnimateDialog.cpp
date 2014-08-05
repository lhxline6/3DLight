// AnimateDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "AnimateDialog.h"
#include "afxdialogex.h"
#include "SceneNodeManager.h"
#include "converter.h"
using namespace irr;
using namespace scene;
// CAnimateDialog 对话框

IMPLEMENT_DYNAMIC(CAnimateDialog, CDialog)

CAnimateDialog::CAnimateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimateDialog::IDD, pParent)
{

}

CAnimateDialog::~CAnimateDialog()
{
}

void CAnimateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DURATION, m_duration);
	DDX_Control(pDX, IDC_START_X, m_startX);
	DDX_Control(pDX, IDC_START_Y, m_startY);
	DDX_Control(pDX, IDC_START_Z, m_startZ);
	DDX_Control(pDX, IDC_END_X, m_endX);
	DDX_Control(pDX, IDC_END_Y, m_endY);
	DDX_Control(pDX, IDC_END_Z, m_endZ);
	DDX_Control(pDX, IDC_CHECK1, m_pingpang);
}


BEGIN_MESSAGE_MAP(CAnimateDialog, CDialog)
	ON_EN_CHANGE(IDC_DURATION, &CAnimateDialog::OnEnChangeDuration)
	ON_BN_CLICKED(IDDELETE, &CAnimateDialog::OnBnClickedDelete)
	ON_EN_CHANGE(IDC_START_X, &CAnimateDialog::OnEnChangeStartX)
	ON_EN_CHANGE(IDC_START_Y, &CAnimateDialog::OnEnChangeStartY)
	ON_EN_CHANGE(IDC_START_Z, &CAnimateDialog::OnEnChangeStartZ)
	ON_EN_CHANGE(IDC_END_X, &CAnimateDialog::OnEnChangeEndX)
	ON_EN_CHANGE(IDC_END_Y, &CAnimateDialog::OnEnChangeEndY)
	ON_EN_CHANGE(IDC_END_Z, &CAnimateDialog::OnEnChangeEndZ)
	ON_BN_CLICKED(IDC_CHECK1, &CAnimateDialog::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CAnimateDialog 消息处理程序


void CAnimateDialog::OnEnChangeDuration()
{
	CString val;
	m_duration.GetWindowText(val);
	m_iTime = (int)stof(val);
}


BOOL CAnimateDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr==NULL)
		return false;
	ISceneNode * node = snmgr->getCurrentNode();
	AnimateInfo info;
	if(node!=NULL)
		info = snmgr->getAnimatedValue();
	CString buf;
	m_bPingpang = info.pingpang;
	m_iTime = info.timeForWay;
	m_fStartX = info.startPoint.X;
	m_fStartY = info.startPoint.Y;
	m_fStartZ = info.startPoint.Z;
	m_fEndX = info.endPoint.X;
	m_fEndY = info.endPoint.Y;
	m_fEndZ = info.endPoint.Z;
	buf.Format(CString("%i"),m_iTime);
	m_duration.SetWindowText(buf);
	m_startX.SetWindowText(ftos(info.startPoint.X));
	m_startY.SetWindowText(ftos(info.startPoint.Y));
	m_startZ.SetWindowText(ftos(info.startPoint.Z));
	m_endX.SetWindowText(ftos(info.endPoint.X));
	m_endY.SetWindowText(ftos(info.endPoint.Y));
	m_endZ.SetWindowText(ftos(info.endPoint.Z));
	m_pingpang.SetCheck(m_bPingpang);
}



void CAnimateDialog::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDDELETE);
}


void CAnimateDialog::OnEnChangeStartX()
{
	CString val;
	m_startX.GetWindowText(val);
	if(isNumber(val)){
		m_fStartX = stof(val);
	}
	else
		m_startX.SetWindowText(ftos(m_fStartX));
}


void CAnimateDialog::OnEnChangeStartY()
{
	CString val;
	m_startY.GetWindowText(val);
	if(isNumber(val)){
		m_fStartY = stof(val);
	}
	else
		m_startY.SetWindowText(ftos(m_fStartY));
}


void CAnimateDialog::OnEnChangeStartZ()
{
	CString val;
	m_startZ.GetWindowText(val);
		if(isNumber(val)){
		m_fStartZ = stof(val);
	}
	else
		m_startZ.SetWindowText(ftos(m_fStartZ));
}


void CAnimateDialog::OnEnChangeEndX()
{
	CString val;
	m_endX.GetWindowText(val);
	if(isNumber(val)){
		m_fEndX = stof(val);
	}
	else
		m_endX.SetWindowText(ftos(m_fEndX));
}


void CAnimateDialog::OnEnChangeEndY()
{
	CString val;
	m_endY.GetWindowText(val);
	if(isNumber(val)){
		m_fEndY = stof(val);
	}
	else
		m_endY.SetWindowText(ftos(m_fEndY));
}


void CAnimateDialog::OnEnChangeEndZ()
{
	CString val;
	m_endZ.GetWindowText(val);
	if(isNumber(val)){
		m_fEndZ = stof(val);
	}
	else
		m_endZ.SetWindowText(ftos(m_fEndZ));
}



void CAnimateDialog::OnBnClickedCheck1()
{
	m_bPingpang = (bool)m_pingpang.GetCheck();
}
