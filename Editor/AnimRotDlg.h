#pragma once
#include "afxwin.h"


// CAnimRotDlg 对话框

class CAnimRotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimRotDlg)

public:
	CAnimRotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnimRotDlg();

// 对话框数据
	enum { IDD = IDD_ANIM_ROT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
