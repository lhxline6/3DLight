// SkyBoxEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "SkyBoxEditor.h"
#include "afxdialogex.h"
#include "BitmapManager.h"
#include "converter.h"
#include<vector>
using namespace Gdiplus;
using namespace std;
// CSkyBoxEditor 对话框

IMPLEMENT_DYNAMIC(CSkyBoxEditor, CDialogEx)

CSkyBoxEditor::CSkyBoxEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSkyBoxEditor::IDD, pParent)
{
	m_strUpPath = 
	m_strDownPath = 
	m_strLeftPath = 
	m_strRightPath = 
	m_strFrontPath = 
	m_strBackPath = CString("");

	m_rectUp = CRect(56,66,171,148);
	m_rectDown = CRect(56,235,171,317);
	m_rectLeft = CRect(232,66,347,148);
	m_rectRight = CRect(232,235,347,317);
	m_rectFront = CRect(390,66,505,148);
	m_rectBack = CRect(390,235,505,317);
}

CSkyBoxEditor::~CSkyBoxEditor()
{
}

void CSkyBoxEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSkyBoxEditor, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SKYBOX_UP, &CSkyBoxEditor::OnBnClickedSkyboxUp)
	ON_BN_CLICKED(IDC_SKYBOX_LEFT, &CSkyBoxEditor::OnBnClickedSkyboxLeft)
	ON_BN_CLICKED(IDC_SKYBOX_DOWN, &CSkyBoxEditor::OnBnClickedSkyboxDown)
	ON_BN_CLICKED(IDC_SKYBOX_RIGHT, &CSkyBoxEditor::OnBnClickedSkyboxRight)
	ON_BN_CLICKED(IDC_SKYBOX_FRONT, &CSkyBoxEditor::OnBnClickedSkyboxFront)
	ON_BN_CLICKED(IDC_SKYBOX_BACK, &CSkyBoxEditor::OnBnClickedSkyboxBack)
END_MESSAGE_MAP()


// CSkyBoxEditor 消息处理程序


void CSkyBoxEditor::OnPaint()
{
	CPaintDC dc(this);
	Graphics graph(dc.GetSafeHdc());
	BitmapManager * bmgr = BitmapManager::GetInstance();
	Bitmap * bitmap;
	vector<CString> vect_path;
	vect_path.push_back(m_strUpPath);
	vect_path.push_back(m_strDownPath);
	vect_path.push_back(m_strLeftPath);
	vect_path.push_back(m_strRightPath);
	vect_path.push_back(m_strFrontPath);
	vect_path.push_back(m_strBackPath);
	vector<Rect> vect_rect;
	vect_rect.push_back(CRect2Rect(m_rectUp));
	vect_rect.push_back(CRect2Rect(m_rectDown));
	vect_rect.push_back(CRect2Rect(m_rectLeft));
	vect_rect.push_back(CRect2Rect(m_rectRight));
	vect_rect.push_back(CRect2Rect(m_rectFront));
	vect_rect.push_back(CRect2Rect(m_rectBack));
	for(int i=0;i<6;i++){
		bitmap = bmgr->GetBitmap(vect_path[i]);
		if(bitmap==NULL){
			bmgr->AddBitmap(vect_path[i]);
			bitmap = bmgr->GetBitmap(vect_path[i]);
		}
		if(bitmap&&bitmap->GetFrameDimensionsCount()>0){
			graph.DrawImage(bitmap,vect_rect[i],
				0,0,bitmap->GetWidth(),bitmap->GetHeight(),UnitPixel);
		}
	}
}


void CSkyBoxEditor::OnBnClickedSkyboxUp()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strUpPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectUp,false);
}


void CSkyBoxEditor::OnBnClickedSkyboxLeft()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strLeftPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectLeft,false);
}


void CSkyBoxEditor::OnBnClickedSkyboxDown()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strDownPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectDown,false);
}


void CSkyBoxEditor::OnBnClickedSkyboxRight()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strRightPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectRight,false);
}


void CSkyBoxEditor::OnBnClickedSkyboxFront()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strFrontPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectFront,false);
}


void CSkyBoxEditor::OnBnClickedSkyboxBack()
{
	CString ext_name("jpg文件|*.jpg|bmp文件|*.bmp|png文件|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()==IDOK){
		m_strBackPath = dlg_file.GetPathName();
	}
	InvalidateRect(m_rectBack,false);
}
