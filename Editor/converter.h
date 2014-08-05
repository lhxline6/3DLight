#ifndef CONVERTER_H
#define CONVERTER_H
#include <string>
#include "stdafx.h"
#include "GdiplusInterface.h"

CString ftos(float);

bool isNumber(const CString &);

float stof(const CString &);

//��CStringת��Ϊ����ΪASCII��string
std::string CString2string(const CString &);

//������ΪASCII��stringת��ΪCString
CString string2CString(const std::string &);

//��CRectת��ΪGdiplus�е�Rect
Gdiplus::Rect CRect2Rect(const CRect &);

#endif
