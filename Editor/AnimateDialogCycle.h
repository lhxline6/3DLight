#pragma once
#include "afxwin.h"


// AnimateDialogCycle �Ի���

class AnimateDialogCycle : public CDialogEx
{
	DECLARE_DYNAMIC(AnimateDialogCycle)

public:
	AnimateDialogCycle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AnimateDialogCycle();

// �Ի�������
	enum { IDD = IDD_ANIM_CYCLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_startX;
	CEdit m_startY;
	CEdit m_startZ;
	CEdit m_radius;
	CEdit m_speed;

	float m_fStartX;
	float m_fStartY;
	float m_fStartZ;
	float m_fRadius;
	float m_fSpeed;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeStartX();
	afx_msg void OnEnChangeStartY();
	afx_msg void OnEnChangeStartZ();
	afx_msg void OnEnChangeRadius();
	afx_msg void OnEnChangeSpeed();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnAddCone();
	afx_msg void OnAddConePop();
	afx_msg void OnAddBillboard();
};
