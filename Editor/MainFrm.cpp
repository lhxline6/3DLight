
// MainFrm.cpp : CMainFrame ���ʵ��
//


#include "stdafx.h"
#include "Editor.h"
#include "EditorView.h"
#include "MainFrm.h"
#include "EditCamera.h"
#include "FileManager.h"
#include "converter.h"
#include "UIListener.h"
#include "AnimateDialog.h"
#include "AnimateDialogCycle.h"
#include "AnimTypeDlg.h"
#include "AnimRotDlg.h"
#include "SkyBoxEditor.h"
#include "converter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace irr;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_ADDTEXTURE, &CMainFrame::OnAddtexture)
	ON_COMMAND(ID_DELTEXTURE, &CMainFrame::OnDeltexture)
	ON_COMMAND(ID_WNDTEXTURE, &CMainFrame::OnWndtexture)
	ON_COMMAND(ID_WNDPROPERTY, &CMainFrame::OnWndproperty)
	ON_COMMAND(ID_WNDNODETREE, &CMainFrame::OnWndnodetree)
	ON_COMMAND(ID_CAMERA_POS, &CMainFrame::OnCameraPos)
	ON_COMMAND(ID_CAMERA_ROT, &CMainFrame::OnCameraRot)
	ON_COMMAND(ID_CAMERA_SCALE, &CMainFrame::OnCameraScale)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_POS, &CMainFrame::OnUpdateCameraPos)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROT, &CMainFrame::OnUpdateCameraRot)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SCALE, &CMainFrame::OnUpdateCameraScale)
	ON_COMMAND(ID_ADD_CUBE, &CMainFrame::OnAddCube)
	ON_COMMAND(ID_ADD_SPHERE, &CMainFrame::OnAddSphere)
	ON_COMMAND(ID_ADD_LIGHT, &CMainFrame::OnAddLight)
	ON_COMMAND(ID_DELETE_CURRENT, &CMainFrame::OnDeleteCurrent)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMainFrame::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVEAS, &CMainFrame::OnFileSaveas)
	ON_COMMAND(ID_FILE_SAV, &CMainFrame::OnFileSav)
	ON_COMMAND(ID_ADD_CUBE_POP, &CMainFrame::OnAddCubePop)
	ON_COMMAND(ID_ADD_SPHERE_POP, &CMainFrame::OnAddSpherePop)
	ON_COMMAND(ID_DELETE_CURRENT_POP, &CMainFrame::OnDeleteCurrentPop)
	ON_COMMAND(ID_ANIM_EDIT, &CMainFrame::OnAnimEdit)
	ON_COMMAND(ID_EXPORT_CODE, &CMainFrame::OnExportCode)
	ON_COMMAND(ID_ADD_CYLINDER, &CMainFrame::OnAddCylinder)
	ON_COMMAND(ID_ADD_CONE, &CMainFrame::OnAddCone)
	ON_COMMAND(ID_ADD_BILLBOARD, &CMainFrame::OnAddBillboard)
	ON_COMMAND(ID_ADD_PARTICLE, &CMainFrame::OnAddParticle)
	ON_COMMAND(ID_RESET_SKYBOX, &CMainFrame::OnResetSkybox)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);



	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	//��ӵĿ��϶��ؼ�
	m_wndProperty.create(this);
	m_wndTexture.create(this);
	m_wndTree.create(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	
}



// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	CEditorView * view = (CEditorView*)GetActiveView();
	if(!view)
		return;
	CRect rect;
	view->Resize();
	// TODO: �ڴ˴������Ϣ����������
}

void CMainFrame::OnFileOpen()
{
	// TODO: �ڴ���������������
	scene::FileManager * fmgr = scene::FileManager::getInstance();
	UIListener * ul = UIListener::getInstance();
	scene::ISceneNodeManager * snmgr = scene::ISceneNodeManager::getInstance();
	CString file_path_name;
	CString ext_name("irr�ļ�|*.irr||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()!=IDOK)
		return;
	CEditorView * pView = (CEditorView*)ul->getCurrentView();
	std::string file_path = CString2string(dlg_file.GetPathName());
	//������ϴε��ļ����������ϴε��ļ�
	if(pView->m_strCurFile.GetLength()!=0)
	{
		UINT res = MessageBox(L"�Ƿ񱣴浱ǰ�ļ���",L"�����ļ�",MB_YESNOCANCEL);
		if(res==IDOK)
		{
			OnFileSav();
		}
		else if(res==IDCANCEL)
			return ;
	}
	pView->DeleteInitialNodes();
	snmgr->clear();
	//ҪΪView��ӳ�ʼ�ڵ�
	
	pView->AddInitialNodes();
	fmgr->loadScene(file_path.c_str());
	ul->sendMessage(UIListener::NodeTreePane);
	//���µ�ǰ���ļ�·����
	pView->m_strCurFile = dlg_file.GetPathName();
}

void CMainFrame::OnAddtexture()
{
	// TODO: �ڴ���������������
	CString file_path_name;
	CString ext_name("jpg�ļ�|*.jpg|bmp�ļ�|*.bmp|png�ļ�|*.png||");
	CFileDialog dlg_file(TRUE,NULL,NULL
		,OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST,ext_name);
	if(dlg_file.DoModal()!=IDOK)
		return;
	POSITION pos = dlg_file.GetStartPosition();
	while(pos)
	{
		file_path_name = dlg_file.GetNextPathName(pos);
		m_wndTexture.m_wndTextureList.AddItem(CTextureList::Node(file_path_name));
	}
}


void CMainFrame::OnDeltexture()
{
	// TODO: �ڴ���������������
	CTextureList * pTextureList = &(m_wndTexture.m_wndTextureList);
	pTextureList->RemoveItem(pTextureList->GetCurSel());
}


void CMainFrame::OnWndtexture()
{
	// TODO: �ڴ���������������
	m_wndTexture.ShowWindow(SW_SHOW);
}




BOOL CMainFrame::OnCloseDockingPane(CDockablePane* pWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWndEx::OnCloseDockingPane(pWnd);
}


BOOL CMainFrame::OnCloseMiniFrame(CPaneFrameWnd* pWnd)
{
	return false;
	// TODO: �ڴ����ר�ô����/����û���
	// return CFrameWndEx::OnCloseMiniFrame(pWnd);
}


void CMainFrame::OnWndproperty()
{
	// TODO: �ڴ���������������
	m_wndProperty.ShowWindow(SW_SHOW);
}


void CMainFrame::OnWndnodetree()
{
	// TODO: �ڴ���������������
	m_wndTree.ShowWindow(SW_SHOW);
}


void CMainFrame::OnCameraPos()
{
	// TODO: �ڴ���������������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(camera)
	{
		scene::setEditCameraState(camera,scene::ICameraAnimator::Move);
	}
}


void CMainFrame::OnCameraRot()
{
	// TODO: �ڴ���������������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(camera)
	{
		scene::setEditCameraState(camera,scene::ICameraAnimator::Rotate);
	}
}


void CMainFrame::OnCameraScale()
{
	// TODO: �ڴ���������������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(camera)
	{
		scene::setEditCameraState(camera,scene::ICameraAnimator::Scale);
	}
}


void CMainFrame::OnUpdateCameraPos(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	if(view==NULL||!view->device)
		return;
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(!camera)
		return;
	scene::ICameraAnimator::NodeState state = 
		scene::getEditCameraState(camera);
	bool res = state==scene::ICameraAnimator::Move;
	pCmdUI->SetCheck(res);
}


void CMainFrame::OnUpdateCameraRot(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	if(view==NULL||!view->device)
		return;
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(!camera)
		return;
	scene::ICameraAnimator::NodeState state = 
		scene::getEditCameraState(camera);
	bool res = state==scene::ICameraAnimator::Rotate;
	pCmdUI->SetCheck(res);
}


void CMainFrame::OnUpdateCameraScale(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CEditorApp * app = (CEditorApp*)::AfxGetApp();
	CEditorView * view = app->GetCurrentView();
	if(view==NULL||!view->device)
		return;
	scene::ICameraSceneNode * camera = 
		view->device->getSceneManager()->getActiveCamera();
	if(!camera)
		return;
	scene::ICameraAnimator::NodeState state = 
		scene::getEditCameraState(camera);
	bool res = state==scene::ICameraAnimator::Scale;
	pCmdUI->SetCheck(res);
}


void CMainFrame::OnAddCube()
{
	scene::ISceneNodeManager * snmgr = 
		scene::ISceneNodeManager::getInstance();
	snmgr->addCube();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnAddSphere()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
		scene::ISceneNodeManager::getInstance();
	snmgr->addSphere();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnAddLight()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
		scene::ISceneNodeManager::getInstance();
	snmgr->addLight();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnDeleteCurrent()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
		scene::ISceneNodeManager::getInstance();
	snmgr->deleteCurrentNode();
	m_wndTree.UpdateTreeNode();
	m_wndProperty.UpdatePropList();
}


void CMainFrame::OnFileSaveAs()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnFileSaveas()
{
	// TODO: �ڴ���������������
	scene::FileManager * fmgr = scene::FileManager::getInstance();
	UIListener * ul = UIListener::getInstance();
	CString file_path_name;
	CString ext_name("irr�ļ�|*.irr||");
	CFileDialog dlg_file(FALSE,NULL,NULL
		,NULL,ext_name);
	if(dlg_file.DoModal()!=IDOK)
		return;
	std::string file_path = CString2string(dlg_file.GetPathName());
	//����ǰҪ�ѻ������
	CEditorView * pView = (CEditorView*)ul->getCurrentView();
	pView->DeleteInitialNodes();
	fmgr->saveScene(file_path.c_str());
	pView->AddInitialNodes();
	//���µ�ǰ���ļ�·����
	pView->m_strCurFile = dlg_file.GetPathName();
}


void CMainFrame::OnFileSav()
{
	UIListener * ul = UIListener::getInstance();
	CEditorView * pView = (CEditorView*)ul->getCurrentView();
	scene::ISceneNodeManager * snmgr = scene::ISceneNodeManager::getInstance();
	if(pView->m_strCurFile.GetLength()==0)
	{
		OnFileSaveas();
		return ;
	}
	scene::FileManager * fmgr = scene::FileManager::getInstance();
	std::string file_path = CString2string(pView->m_strCurFile);
	//����ǰҪ�ѻ������
	snmgr->setCurrentNode(NULL);
	pView->DeleteInitialNodes();
	fmgr->saveScene(file_path.c_str());
	pView->AddInitialNodes();
}



void CMainFrame::OnAddCubePop()
{
	// TODO: �ڴ���������������
	OnAddCube();
}

void CMainFrame::AddCube()
{
	OnAddCube();
}
void CMainFrame::AddSphere()
{
	OnAddSphere();
}
void CMainFrame::DeleteCurrent()
{
	OnDeleteCurrent();
}


void CMainFrame::OnAddSpherePop()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnDeleteCurrentPop()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnAnimEdit()
{
	scene::ISceneNodeManager * snmgr = scene::ISceneNodeManager::getInstance();
	if(!snmgr)
		return ;
	scene::ISceneNode * node = snmgr->getCurrentNode();
	if(node==NULL)
	{
		MessageBox(L"û��ѡ��һ���ڵ�",L"����ʧ��",MB_OK);
		return;
	}
	scene::AnimateInfo info;
	if(snmgr->hasAnimator()==false)
	{
		UINT res = MessageBox(L"��ǰ�ڵ�û�ж�����ҪΪ�����һ������ô��",L"��Ӷ���",MB_YESNO);
		if(res==IDNO)
			return ;
		else{
			CAnimTypeDlg typeDlg;
			UINT res = typeDlg.DoModal();
			if(res==IDCANCEL)
				return ;
			if(typeDlg.m_strType==CString("line")){
				snmgr->setStraightFly(node->getAbsolutePosition(),node->getAbsolutePosition());
			}
			else if(typeDlg.m_strType==CString("cycle")){
				snmgr->setCycleFly(node->getAbsolutePosition());
			}
			else if(typeDlg.m_strType==CString("rotate")){
				snmgr->setRotation();
			}
		}
	}
	info = snmgr->getAnimatedValue();
	UINT res;
	if(info.animateType==scene::AT_STRAIGHT){
		CAnimateDialog anim_dlg;
		res = anim_dlg.DoModal();
		if(res==IDOK){
			info.timeForWay = anim_dlg.m_iTime;
			info.startPoint.X = anim_dlg.m_fStartX;
			info.startPoint.Y = anim_dlg.m_fStartY;
			info.startPoint.Z = anim_dlg.m_fStartZ;
			info.endPoint.X = anim_dlg.m_fEndX;
			info.endPoint.Y = anim_dlg.m_fEndY;
			info.endPoint.Z = anim_dlg.m_fEndZ;
			info.pingpang = anim_dlg.m_bPingpang;

			snmgr->setAnimatedValue(info);
		}
	}
	else if(info.animateType==scene::AT_CYCLE){
		AnimateDialogCycle anim_dlg;
		res = anim_dlg.DoModal();
		if(res==IDOK){
			info.center.X = anim_dlg.m_fStartX;
			info.center.Y = anim_dlg.m_fStartY;
			info.center.Z = anim_dlg.m_fStartZ;
			info.radius = anim_dlg.m_fRadius;
			info.speed = anim_dlg.m_fSpeed;

			snmgr->setAnimatedValue(info);
		}
	}
	else if(info.animateType==scene::AT_ROTATION){
		CAnimRotDlg anim_dlg;
		res = anim_dlg.DoModal();
		if(res==IDOK){
			info.rotationSpeed = 
				core::vector3df(anim_dlg.m_fSpeedX,
									anim_dlg.m_fSpeedY,
									anim_dlg.m_fSpeedZ);
			snmgr->setAnimatedValue(info);
		}
	}
	if(res==IDCANCEL)
		return;
	else if(res==IDDELETE)
	{
		snmgr->removeAnimator();
		return;
	}
}


void CMainFrame::OnExportCode()
{
	scene::FileManager * fmgr = scene::FileManager::getInstance();
	CString file_path_name;
	CString ext_name("cpp�ļ�|*.cpp||");
	CFileDialog dlg_file(FALSE,NULL,NULL
		,NULL,ext_name);
	if(dlg_file.DoModal()!=IDOK)
		return;
	std::string file_path = CString2string(dlg_file.GetPathName());
	fmgr->exportCode(file_path);
	MessageBox(L"������ɣ�",L"�ɹ�",MB_OK);
}


void CMainFrame::OnAddCylinder()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
	scene::ISceneNodeManager::getInstance();
	snmgr->addCylinder();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnAddCone()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
	scene::ISceneNodeManager::getInstance();
	snmgr->addCone();
	m_wndTree.UpdateTreeNode();
}



void CMainFrame::OnAddBillboard()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
	scene::ISceneNodeManager::getInstance();
	snmgr->addBillboardText();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnAddParticle()
{
	// TODO: �ڴ���������������
	scene::ISceneNodeManager * snmgr = 
	scene::ISceneNodeManager::getInstance();
	snmgr->addParticleSystem();
	m_wndTree.UpdateTreeNode();
}


void CMainFrame::OnResetSkybox()
{
	CSkyBoxEditor skybox;
	scene::ISceneNodeManager * snmgr = 
	scene::ISceneNodeManager::getInstance();
	if(skybox.DoModal()==IDOK){
		scene::ISceneNode * node = snmgr->m_pSkyBox;
		if(node)
			node->remove();
		snmgr->addSkyBox(
			CString2string(skybox.m_strUpPath),
			CString2string(skybox.m_strDownPath),
			CString2string(skybox.m_strLeftPath),
			CString2string(skybox.m_strRightPath),
			CString2string(skybox.m_strFrontPath),
			CString2string(skybox.m_strBackPath));
	}
}
