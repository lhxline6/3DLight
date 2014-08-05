
// Editor.h : Editor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEditorApp:
// �йش����ʵ�֣������ Editor.cpp
//
class CEditorView;
class CEditorApp : public CWinAppEx
{
private:
	CEditorView * m_pCurrentView;
public:
	CEditorApp();

	void SetCurrentView(CEditorView* view);
	CEditorView * GetCurrentView();
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int Run();
	void update(void);
	virtual BOOL OnIdle(LONG lCount);
};

extern CEditorApp theApp;
