#pragma once


// CNodeTreeCtrl
class CNodeTreePane;
class CNodeTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CNodeTreeCtrl)

private:
	CNodeTreePane * m_pParent;
public:
	CNodeTreeCtrl();
	virtual ~CNodeTreeCtrl();
	
	void BindParent(CNodeTreePane * parent);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


