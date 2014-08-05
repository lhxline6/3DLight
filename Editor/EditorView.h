
// EditorView.h : CEditorView 类的接口
//
#include "SceneNodeManager.h"
#pragma once
using namespace irr;

class CEditorView : public CView
{
protected: // 仅从序列化创建
	CEditorView();
	DECLARE_DYNCREATE(CEditorView)

// 特性
public:
	CString m_strCurFile;
public:
//---------------------------功能成员----------------------------
	IrrlichtDevice* device;
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;
	scene::ICameraSceneNode* cam;;
	SIrrlichtCreationParameters param;
	scene::ISceneNodeAnimator* anim;
	scene::ILightSceneNode * enlight[8];
	bool m_run;
//-----------------------------------------------------------------

//----------------------------消息处理函数--------------------------
	static void OnPropertyPane(void *);
	static void OnNodeTreePane(void *);
	static void OnInitialScene(void *);
//--------------------------------------------------------------------
// 操作
public:
	void AddInitialNodes();
	void DeleteInitialNodes();
	void Resize();
	void GlobalRender();
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void update(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddCubePop();
	afx_msg void OnAddSpherePop();
	afx_msg void OnDeleteCurrentPop();
	afx_msg void OnAnimEdit();
	afx_msg void OnCameraPos();
	afx_msg void OnCameraRot();
	afx_msg void OnCameraScale();
	afx_msg void OnUpdateCameraPos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCameraRot(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCameraScale(CCmdUI *pCmdUI);
	afx_msg void OnAddCylniderPop();
	afx_msg void OnAddCone();
	afx_msg void OnAddConePop();
	afx_msg void OnAddParticlePop();
	afx_msg void OnAddBillboardPop();
};

#ifndef _DEBUG  // EditorView.cpp 中的调试版本

#endif

