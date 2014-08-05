#pragma once
#include "afxwin.h"


// CAnimRotDlg �Ի���

class CAnimRotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimRotDlg)

public:
	CAnimRotDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnimRotDlg();

// �Ի�������
	enum { IDD = IDD_ANIM_ROT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_speedX;
	CEdit m_speedY;
	CEdit m_speedZ;
	float m_fSpeedX;
	float m_fSpeedY;
	float m_fSpeedZ;
	afx_msg void OnEnChangeSpeedX();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeSpeedY();
	afx_msg void OnEnChangeStartZ();
	afx_msg void OnBnClickedDelete();
};
