#pragma once
#include "afxwin.h"


// CAnimTypeDlg �Ի���

class CAnimTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimTypeDlg)

public:
	CAnimTypeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnimTypeDlg();

// �Ի�������
	enum { IDD = IDD_ANIM_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_type;
	CString m_strType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
};
