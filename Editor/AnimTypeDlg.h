#pragma once
#include "afxwin.h"


// CAnimTypeDlg 对话框

class CAnimTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimTypeDlg)

public:
	CAnimTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnimTypeDlg();

// 对话框数据
	enum { IDD = IDD_ANIM_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_type;
	CString m_strType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
};
