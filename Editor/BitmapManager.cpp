#include "stdafx.h"
#include "BitmapManager.h"
#include <cstring>

using namespace std;
using namespace Gdiplus;


BitmapManager * BitmapManager::instance  = NULL;

BitmapManager::BitmapManager()
{}
BitmapManager::~BitmapManager()
{
	DeleteAll();
}

void BitmapManager::DeleteAll()
{
	BitmapStore::iterator iter;
	for(iter = m_store.begin();iter != m_store.end(); iter++)
	{
		delete (Bitmap *)(iter->second);
	}
}

BitmapManager * BitmapManager::GetInstance()
{
	if(instance)
		return instance;
	return (instance = new BitmapManager());
}

BOOL BitmapManager::AddBitmap(const CString & path)
{
	BitmapStore::iterator iter = m_store.find(path);
	if(iter!=m_store.end())
		return TRUE;
	Bitmap * pBitmap = Bitmap::FromFile(path);
	if(!pBitmap->GetFrameDimensionsCount())
		return FALSE;
	m_store.insert(pair<CString,UINT>(path,(UINT)(pBitmap)));
	return TRUE;
}
Bitmap * BitmapManager::GetBitmap(const CString & path)
{
	BitmapStore::iterator iter = m_store.find(path);
	if(iter==m_store.end())
		return NULL;
	Bitmap * result = (Bitmap*)iter->second;
	return result;
}

void BitmapManager::DeleteBitmap(const CString & path)
{
	BitmapStore::iterator iter = m_store.find(path);
	if(iter!=m_store.end())
	{
		delete (Bitmap*)iter->second;
		m_store.erase(iter);
	}
}
