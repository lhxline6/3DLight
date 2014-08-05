// NodeTreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Editor.h"
#include "NodeTreeCtrl.h"
#include "NodeTreePane.h"

// CNodeTreeCtrl

IMPLEMENT_DYNAMIC(CNodeTreeCtrl, CTreeCtrl)

CNodeTreeCtrl::CNodeTreeCtrl()
{
}

CNodeTreeCtrl::~CNodeTreeCtrl()
{
}
void CNodeTreeCtrl::BindParent(CNodeTreePane * parent)
{
	m_pParent = parent;
}

BEGIN_MESSAGE_MAP(CNodeTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CNodeTreeCtrl::OnTvnItemChanged)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CNodeTreeCtrl ��Ϣ�������



void CNodeTreeCtrl::OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pParent->OnSelChange();
	*pResult = 0;
}


void CNodeTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_pParent)
	{
		m_pParent->OnSelChange();
	}
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
