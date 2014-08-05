#pragma once
#include "afxwin.h"


// CAnimateDialog 对话框

class CAnimateDialog : public CDialog
{
	DECLARE_DYNAMIC(CAnimateDialog)

public:
	CAnimateDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnimateDialog();

// 对话框数据
	enum { IDD = IDD_ANIM_CUBE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeDuration();
	CEdit m_duration;
	CEdit m_startX;
	CEdit m_startY;
	CEdit m_startZ;
	CEdit m_endX;
	CEdit m_endY;
	CEdit m_endZ;
	CButton m_pingpang;

	int m_iTime;
	float m_fStartX;
	float m_fStartY;
	float m_fStartZ;
	float m_fEndX;
	float m_fEndY;
	float m_fEndZ;
	bool m_bPingpang;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnEnChangeStartX();
	afx_msg void OnEnChangeStartY();
	afx_msg void OnEnChangeStartZ();
	afx_msg void OnEnChangeEndX();
	afx_msg void OnEnChangeEndY();
	afx_msg void OnEnChangeEndZ();
	
	afx_msg void OnBnClickedCheck1();
};
