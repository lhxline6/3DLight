// AnimTypeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "AnimTypeDlg.h"
#include "afxdialogex.h"


// CAnimTypeDlg 对话框

IMPLEMENT_DYNAMIC(CAnimTypeDlg, CDialogEx)

CAnimTypeDlg::CAnimTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnimTypeDlg::IDD, pParent)
{

}

CAnimTypeDlg::~CAnimTypeDlg()
{
}

void CAnimTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_type);
}


BEGIN_MESSAGE_MAP(CAnimTypeDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimTypeDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CAnimTypeDlg 消息处理程序


BOOL CAnimTypeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_type.AddString(L"直线移动");
	m_type.AddString(L"环绕移动");
	m_type.AddString(L"自旋转");
	m_type.SetCurSel(m_type.FindString(-1,L"直线移动"));
	m_strType = CString("line");
	return true;
}


void CAnimTypeDlg::OnCbnSelchangeCombo1()
{
	CString val;
	int sel = m_type.GetCurSel();
	if(sel==-1)
			return;
	m_type.GetLBText(sel,val);
	if(val==CString("直线移动")){
		m_strType = CString("line");
	}
	else if(val==CString("环绕移动")){
		m_strType = CString("cycle");
	}
	else if(val==CString("自旋转")){
		m_strType = CString("rotate");
	}
}

