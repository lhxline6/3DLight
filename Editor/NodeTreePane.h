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
	//默认值较多的创建接口，针对TreeNode
	BOOL create(CFrameWndEx * parent);
	void OnSelChange();
	//更新数据
	void UpdateTreeNode();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

