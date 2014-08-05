#pragma once

#include "TextureList.h"
// CTexturePane
class CFrameWndEx;
class CTexturePane : public CDockablePane
{
	DECLARE_DYNAMIC(CTexturePane)

public:
	CTexturePane();
	virtual ~CTexturePane();

	BOOL create(CFrameWndEx * parent);

	CTextureList m_wndTextureList;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
//	afx_msg void OnDestroy();
};


