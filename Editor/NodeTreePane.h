#pragma once
#include "afxdockablepane.h"
#include "NodeTreeCtrl.h"
#include "irrlicht.h"
#include <vector>

class CNodeTreePane :
	public CDockablePane
{
private:
	struct Node
	{
		HTREEITEM hItem;
		irr::scene::ISceneNode * pSceneNode;
		Node(HTREEITEM h=NULL, irr::scene::ISceneNode * p=NULL):
			hItem(h),pSceneNode(p){}
	};
	std::vector<Node> m_storage;
public:
	CNodeTreeCtrl m_wndNodeTree;
public:
	CNodeTreePane(void);
	~CNodeTreePane(void);
public:
	//Ĭ��ֵ�϶�Ĵ����ӿڣ����TreeNode
	BOOL create(CFrameWndEx * parent);
	void OnSelChange();
	//��������
	void UpdateTreeNode();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

