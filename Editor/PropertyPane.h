#pragma once
#include "afxdockablepane.h"
#include "Irrlicht.h"
#include<vector>

enum ID
{
	P_X, P_Y, P_Z,
	S_X, S_Y, S_Z,
	R_X, R_Y, R_Z,
	C_A, C_R, C_G, C_B,
	E_LIGHT,
	RADIUS,
	NAME
};

struct IdProperty
{
	CMFCPropertyGridProperty * pProp;
	ID pId;
};


class CPaneCtrl:public CMFCPropertyGridCtrl
{
public:
	std::vector<IdProperty>idPropList;
public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
};

class CPropertyPane :
	public CDockablePane
{
public:
	//�ؼ�
	CPaneCtrl m_wndPropList;
public:
	CPropertyPane(void);
	~CPropertyPane(void);
public:
	//�����������������������б�
	void UpdatePropList();
public:
	//Ĭ��ֵ�϶���ⲿ�ӿڣ����Property
	BOOL create(CFrameWndEx * parent);
public:
	//��Ϣ����
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

