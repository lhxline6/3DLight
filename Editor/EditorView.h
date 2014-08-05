
// EditorView.h : CEditorView ��Ľӿ�
//
#include "SceneNodeManager.h"
#pragma once
using namespace irr;

class CEditorView : public CView
{
protected: // �������л�����
	CEditorView();
	DECLARE_DYNCREATE(CEditorView)

// ����
public:
	CString m_strCurFile;
public:
//---------------------------���ܳ�Ա----------------------------
	IrrlichtDevice* device;
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;
	scene::ICameraSceneNode* cam;;
	SIrrlichtCreationParameters param;
	scene::ISceneNodeAnimator* anim;
	scene::ILightSceneNode * enlight[8];
	bool m_run;
//-----------------------------------------------------------------

//----------------------------��Ϣ������--------------------------
	static void OnPropertyPane(void *);
	static void OnNodeTreePane(void *);
	static void OnInitialScene(void *);
//--------------------------------------------------------------------
// ����
public:
	void AddInitialNodes();
	void DeleteInitialNodes();
	void Resize();
	void GlobalRender();
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // EditorView.cpp �еĵ��԰汾

#endif

