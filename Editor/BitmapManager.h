#ifndef BITMAPMANAGER_H
#define BITMAPMANAGER_H
#include "stdafx.h"
#include <map>
typedef std::map<CString,UINT> BitmapStore;
//This class will be a singleton in the project
class BitmapManager
{
private:
	BitmapStore m_store;
	static BitmapManager * instance;
	BitmapManager();
	~BitmapManager();
public:
	static BitmapManager * GetInstance();
	BOOL AddBitmap(const CString & path);
	void DeleteBitmap(const CString & path);
	Gdiplus::Bitmap * GetBitmap(const CString & path);
	void DeleteAll();
};
#endif

