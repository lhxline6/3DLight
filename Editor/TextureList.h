#pragma once

#include<vector>
#include "stdafx.h"
// CTextureList

class CTextureList : public CWnd
{
	DECLARE_DYNAMIC(CTextureList)

public:
	struct Node
	{
		CString m_strFilePath;
		CRect m_rectElement;
		CDC * m_pDC;
		int m_id;
		Node(const CString & path=L"")
		{
			m_strFilePath = path;
		}
	};
	//attributes
	int m_iElemWidth;
	int m_iElemHeight;
private:
	int m_iCurSel;
	std::vector<Node> m_storage;
	CScrollBar m_scroll;

	int updateSel(const CPoint & point);
	void drawElement(const Node & node);
public:
	CTextureList();
	virtual ~CTextureList();

	//operations
public:
	int GetCurSel()const;
	void SetCurSel(int id);
	bool AddItem(Node path);
	void RemoveItem(int id);
	CString GetItem(int id);
	void Resize();
	void Apply();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

