#ifndef CONVERTER_H
#define CONVERTER_H
#include <string>
#include "stdafx.h"
#include "GdiplusInterface.h"

CString ftos(float);

bool isNumber(const CString &);

float stof(const CString &);

//将CString转换为编码为ASCII的string
std::string CString2string(const CString &);

//将编码为ASCII的string转换为CString
CString string2CString(const std::string &);

//将CRect转换为Gdiplus中的Rect
Gdiplus::Rect CRect2Rect(const CRect &);

#endif
