// AnimTypeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Editor.h"
#include "AnimTypeDlg.h"
#include "afxdialogex.h"


// CAnimTypeDlg �Ի���

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


// CAnimTypeDlg ��Ϣ�������


BOOL CAnimTypeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_type.AddString(L"ֱ���ƶ�");
	m_type.AddString(L"�����ƶ�");
	m_type.AddString(L"����ת");
	m_type.SetCurSel(m_type.FindString(-1,L"ֱ���ƶ�"));
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
	if(val==CString("ֱ���ƶ�")){
		m_strType = CString("line");
	}
	else if(val==CString("�����ƶ�")){
		m_strType = CString("cycle");
	}
	else if(val==CString("����ת")){
		m_strType = CString("rotate");
	}
}

