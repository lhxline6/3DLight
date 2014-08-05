
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "PropertyPane.h"
#include "NodeTreePane.h"
#include "TexturePane.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
// 操作
	void AddCube();
	void AddSphere();
	void DeleteCurrent();
public:
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  
	// 控件条嵌入成员
	CPropertyPane m_wndProperty;
	CNodeTreePane m_wndTree;
	CTexturePane m_wndTexture;
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
	afx_msg void OnAddtexture();
	afx_msg void OnDeltexture();
	afx_msg void OnWndtexture();
	virtual BOOL OnCloseDockingPane(CDockablePane* pWnd);
	virtual BOOL OnCloseMiniFrame(CPaneFrameWnd* pWnd);
	afx_msg void OnWndproperty();
	afx_msg void OnWndnodetree();
	afx_msg void OnCameraPos();
	afx_msg void OnCameraRot();
	afx_msg void OnCameraScale();
	afx_msg void OnUpdateCameraPos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCameraRot(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCameraScale(CCmdUI *pCmdUI);
	afx_msg void OnAddCube();
	afx_msg void OnAddSphere();
	afx_msg void OnAddLight();
	afx_msg void OnDeleteCurrent();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileSav();
	afx_msg void OnAddCubePop();
	afx_msg void OnAddSpherePop();
	afx_msg void OnDeleteCurrentPop();
	afx_msg void OnAnimEdit();
	afx_msg void OnExportCode();
	afx_msg void OnAddCylinder();
	afx_msg void OnAddCone();
	afx_msg void OnAddBillboard();
	afx_msg void OnAddParticle();
	afx_msg void OnResetSkybox();
};


