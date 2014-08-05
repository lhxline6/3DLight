#include "stdafx.h"
#include "converter.h"

CString ftos(float f)
{
	CString res;
	res.Format(_T("%.4f"),f);
	return res;
}
bool isNumber(const CString & s)
{
	int fcnt = 0;
	for(int i=0;s[i];i++)
	{
		if(s[i]>='0'&&s[i]<='9')
			continue;
		else if(s[i]=='.')
			fcnt++;
		else if(s[i]=='+'||s[i]=='-')
		{
			if(i==0)
				continue;
			else return false;
		}
		else
			return false;
	}
	if(fcnt>1)
		return false;
	return true;
}
float stof(const CString & cstr)
{
	CString s = cstr;
	s.Trim();
	int point = -1;
	int start = 0;
	float sign = 1.f;
	if(s[0]=='-')
	{
		start = 1;
		sign = -1.f;
	}
	for(int i=0;s[i];i++)
		if(s[i]=='.')
		{
			point = i;
			break;
		}
	if(point==-1)
	{
		float res = 0;
		for(int i=start;s[i];i++)
			res = res*10+(int)(s[i]-'0');
		return res*sign;
	}
	else
	{
		float f1=0,f2=0;
		for(int i=start;i<point;i++)
			f1 = f1*10 + (int)(s[i]-'0');
		for(int i=s.GetLength()-1;i>point;i--)
			f2 = f2/10 + (int)(s[i]-'0');
		f2 /=10;
		return (f1+f2)*sign;
	}
}

CString string2CString(const std::string & str)
{
	WCHAR * pText;
	int iTextLen;
	iTextLen = MultiByteToWideChar(CP_ACP,
		0,str.c_str(),-1,NULL,0);
	pText = new WCHAR[iTextLen+1];
	memset((void*)pText,0,sizeof(char)*(iTextLen+1));
	MultiByteToWideChar(CP_ACP,
		0,str.c_str(),-1,pText,iTextLen);
	CString cstr(pText);
	delete[] pText;
	return cstr;
}

std::string CString2string(const CString & cstr)
{
	char *pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,
         0,
         cstr,
         -1,
         NULL,
         0,
         NULL,
         NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,
         0,
         cstr,
         -1,
         pElementText,
         iTextLen,
         NULL,
         NULL );
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

Gdiplus::Rect CRect2Rect(const CRect & crect)
{
	return Gdiplus::Rect(crect.left,crect.top,crect.Width(),crect.Height());
}

