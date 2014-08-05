#include "stdafx.h"
#include "GdiplusInterface.h"
using namespace Gdiplus;
GdiplusInterface * GdiplusInterface::instance = NULL;

GdiplusInterface::GdiplusInterface()
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}
GdiplusInterface::~GdiplusInterface()
{
	GdiplusShutdown(m_pGdiToken);
}

void GdiplusInterface::StartInstance()
{
	if(!instance)
		instance = new GdiplusInterface();
}
