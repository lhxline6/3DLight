#ifndef GDIPLUSINTERFACE_H
#define GDIPLUSINTERFACE_H
#include "stdafx.h"
class GdiplusInterface
{
private:
	static GdiplusInterface * instance;
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusInterface();
	~GdiplusInterface();
public:
	static void StartInstance();
};

#endif