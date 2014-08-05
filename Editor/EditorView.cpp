
// EditorView.cpp : CEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")

#include "EditorDoc.h"
#include "EditorView.h"
#include "PropertyPane.h"
#include "MainFrm.h"
#include "EditCamera.h"
#include "UIListener.h"
#include "FileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_CUBE_POP, &CEditorView::OnAddCubePop)
	ON_COMMAND(ID_ADD_SPHERE_POP, &CEditorView::OnAddSpherePop)
	ON_COMMAND(ID_DELETE_CURRENT_POP, &CEditorView::OnDeleteCurrentPop)
	ON_COMMAND(ID_ANIM_EDIT, &CEditorView::OnAnimEdit)
	ON_COMMAND(ID_CAMERA_POS, &CEditorView::OnCameraPos)
	ON_COMMAND(ID_CAMERA_ROT, &CEditorView::OnCameraRot)
	ON_COMMAND(ID_CAMERA_SCALE, &CEditorView::OnCameraScale)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_POS, &CEditorView::OnUpdateCameraPos)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROT, &CEditorView::OnUpdateCameraRot)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SCALE, &CEditorView::OnUpdateCameraScale)
	ON_COMMAND(ID_ADD_CYLNIDER_POP, &CEditorView::OnAddCylniderPop)
	ON_COMMAND(ID_ADD_CONE, &CEditorView::OnAddCone)
	ON_COMMAND(ID_ADD_CONE_POP, &CEditorView::OnAddConePop)
	ON_COMMAND(ID_ADD_PARTICLE_POP, &CEditorView::OnAddParticlePop)
	ON_COMMAND(ID_ADD_BILLBOARD_POP, &CEditorView::OnAddBillboardPop)
END_MESSAGE_MAP()


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// CEditorView 构造/析构

CEditorView::CEditorView()
{
	// TODO: 在此处添加构造代码
	m_run = false;
	device = NULL;
}

CEditorView::~CEditorView()
{
	device->drop();
}

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CEditorView 绘制

void CEditorView::OnDraw(CDC* /*pDC*/)
{
	
	// TODO: 在此处为本机数据添加绘制代码
}


// CEditorView 打印


void CEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEditorView 诊断

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG


// CEditorView 消息处理程序


void CEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	//设定当前文件名为空
	m_strCurFile = "";

	CRect client;
	GetClientRect(client);
	// TODO: Add your specialized code here and/or call the base class
	param.WindowId = reinterpret_cast<void*>(this->GetSafeHwnd());
	param.DriverType = irr::video::EDT_DIRECT3D9;
	param.WindowSize = dimension2du(client.Width(),client.Height());
	device = irr::createDeviceEx(param);
	device->setResizable(true);
	driver=device->getVideoDriver();
	smgr=device->getSceneManager();
	
	//清空环境顶点
	cam = NULL;
	for(int i=0;i<8;i++)
		enlight[i] = NULL;
	//创建节点管理器
	ISceneNodeManager::initial(device);
	ISceneNodeManager::clear();
	//创建文件管理器
	FileManager::initial(device);
	/*对需要SceneNodeManager的UI做初始化通知*/
	CMainFrame * pFrame = (CMainFrame*)this->GetParent();
	pFrame->m_wndTree.UpdateTreeNode();
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	
	//天空盒

	smgr->addSkyBoxSceneNode(
	driver->getTexture("media/irrlicht2_up.jpg"),
	driver->getTexture("media/irrlicht2_dn.jpg"),
	driver->getTexture("media/irrlicht2_lf.jpg"),
	driver->getTexture("media/irrlicht2_rt.jpg"),
	driver->getTexture("media/irrlicht2_ft.jpg"),
	driver->getTexture("media/irrlicht2_bk.jpg"));
	//添加初始的环境节点
	AddInitialNodes();

	/*消息传递、刷新等逻辑任务的初始化工作*/
	//在app中添加View使其逐帧刷新
	CEditorApp * pApp = (CEditorApp*)AfxGetApp();
	pApp->SetCurrentView(this);
	CMainFrame * frame = (CMainFrame*)pApp->GetMainWnd();
	frame->m_wndProperty.UpdatePropList();
	//为UIListener注册窗口,绑定消息
	UIListener * ul = UIListener::getInstance();
	ul->activate(this);
	ul->bind(UIListener::PropertyPane,OnPropertyPane);
	ul->bind(UIListener::NodeTreePane,OnNodeTreePane);
	ul->bind(UIListener::InitialScene,OnInitialScene);
	ul->sendMessage(UIListener::NodeTreePane);
	//开始
	m_run = true;
}


void CEditorView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnRButtonDblClk(nFlags, point);
}


void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDblClk(nFlags, point);
}



void CEditorView::update(void)
{
	if(m_run)
	{
		/*外部更新工作*/;
		UIListener::getInstance()->update();
		/*引擎逻辑以及渲染*/
		device->getTimer()->tick();
		driver->beginScene(true, true, 0);
		//======================================================
		smgr->drawAll();
		GlobalRender();
		//======================================================
		
		driver->endScene();
		CMainFrame * frame = (CMainFrame*)this->GetParentFrame();
		frame->SetWindowTextW(L"3DLight");
	}
}

void CEditorView::OnPropertyPane(void * view)
{
	CEditorApp * app = (CEditorApp*)AfxGetApp();
	CMainFrame * main_frame = (CMainFrame*)app->GetMainWnd();
	main_frame->m_wndProperty.UpdatePropList();
}
void CEditorView::OnNodeTreePane(void * view)
{
	CEditorApp * app = (CEditorApp*)AfxGetApp();
	CMainFrame * main_frame = (CMainFrame*)app->GetMainWnd();
	main_frame->m_wndTree.UpdateTreeNode();
}
void CEditorView::OnInitialScene(void * view)
{
	CEditorView * pView = (CEditorView *)view;
	pView->AddInitialNodes();
}

void CEditorView::AddInitialNodes()
{
	if(device==NULL)
		return ;
	//添加前要确保没有环境
	DeleteInitialNodes();
	//摄像机
	cam = addEditCameraSceneNode(device);


	//漫反射环境光
	for(int i=0;i<8;i++)
	{
		enlight[i] = smgr->addLightSceneNode();
		enlight[i]->setRadius(300.f);
		video::SLight data;
		data.DiffuseColor = video::SColorf(0.18f,0.18f,0.18f);
		data.Radius = 900.f;
		enlight[i]->setLightData(data);
	}
	
}
void CEditorView::DeleteInitialNodes()
{
	if(cam)
	{
		cam->remove();
		cam = NULL;
	}
	for(int i=0;i<8;i++)
	{
		if(enlight[i])
		{
			enlight[i]->remove();
			enlight[i] = NULL;
		}
	}
}

void CEditorView::Resize()
{
	if(device)
	{
		CRect client;
		GetClientRect(&client);
		driver->OnResize(dimension2du(client.Width(),client.Height()));
	}
}
void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码
	CView::OnSize(nType, cx, cy);
	Resize();
}

void CEditorView::GlobalRender()
{
	if(cam==NULL)
		return ;
	//画物体移动轴
	do{
		core::vector3df v3df_dir(0,0,0);
		scene::ICameraAnimator::DragState axis = 
			scene::getEditCameraDragDir(cam);
		if(axis==scene::ICameraAnimator::X)
		{
			v3df_dir.X  = 1.f;
		}
		else if(axis==scene::ICameraAnimator::Y)
		{
			v3df_dir.Y = 1.f;
		}
		else if(axis==scene::ICameraAnimator::Z)
		{
			v3df_dir.Z = 1.f;
		}
		if(axis==scene::ICameraAnimator::NONE)
			break;
		ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
		ISceneNode * node = snmgr->getCurrentNode();
		if(node==NULL)
			break;
		vector3df scale = node->getScale();
		float factor = max(abs(scale.X),abs(scale.Y));
		factor = max(factor,abs(scale.Z));
		factor *= 30;
		v3df_dir.setLength(factor);
		vector3df start = node->getAbsolutePosition();
		//如果当前是伸缩状态，那么轴要旋转
		scene::ICameraAnimator::NodeState ns = 
			scene::getEditCameraState(cam);
		if(ns==scene::ICameraAnimator::Scale)
		{
			vector3df rot = node->getRotation();
			core::matrix4 rot_mat;
			rot_mat.setRotationDegrees(rot);
			rot_mat.transformVect(v3df_dir);
		}
		vector3df end = start + v3df_dir;
		start -= v3df_dir;
		video::SMaterial material;
		material.setTexture(0,0);
		material.Lighting = false;
		material.Wireframe = false;
		driver->setMaterial(material);
		driver->setTransform(ETS_WORLD,core::IdentityMatrix);
		driver->draw3DLine(start,end,video::SColor(0,255,0,0));
	}while(false);
	//画地板
	vector3df cam_pos = cam->getAbsolutePosition();
	int mw = 30;//网格的宽度
	video::SMaterial material;
	material.setTexture(0,0);
	material.Lighting = false;
	material.Wireframe = true;
	driver->setMaterial(material);
	driver->setTransform(ETS_WORLD,core::IdentityMatrix);
	vector3df start,end;
	//画X方向的轴
	start.Y = -20;
	end.Y = -20;
	int sx = mw*((int)(cam_pos.X)/mw);
	int sz = mw*((int)(cam_pos.Z)/mw);
	//沿着X画
	start.Z = sz-600;
	end.Z = sz+600;
	for(int i=-600;i<600;i+=mw)
	{
		end.X = start.X = sx+i;
		driver->draw3DLine(start,end,video::SColor(0,230,230,230));
	}
	//沿着Z画
	start.X = sx-600;
	end.X = sx+600;
	for(int i=-600;i<600;i+=mw)
	{
		end.Z = start.Z = sz+i;
		driver->draw3DLine(start,end,video::SColor(0,230,230,230));
	}
	//绘制环境光
	if(enlight[0]==NULL)
		return ;
	static int direction[8][3] = 
	{
		{-1,-1,-1},
		{-1,-1,1},
		{-1,1,-1},
		{-1,1,1},
		{1,-1,-1},
		{1,-1,1},
		{1,1,-1},
		{1,1,1}
	};
	for(int i=0;i<8;i++)
	{
		vector3df light_pos = cam_pos +
			vector3df(
			300*direction[i][0],
			300*direction[i][1],
			300*direction[i][2]);
		enlight[i]->setPosition(light_pos);
	}
}




void CEditorView::OnAddCubePop()
{
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->AddCube();
}


void CEditorView::OnAddSpherePop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->AddSphere();
}


void CEditorView::OnDeleteCurrentPop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->DeleteCurrent();
}


void CEditorView::OnAnimEdit()
{
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAnimEdit();	
}


void CEditorView::OnCameraPos()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnCameraPos();
}


void CEditorView::OnCameraRot()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnCameraRot();
}


void CEditorView::OnCameraScale()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnCameraScale();
}


void CEditorView::OnUpdateCameraPos(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnUpdateCameraPos(pCmdUI);
}


void CEditorView::OnUpdateCameraRot(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnUpdateCameraRot(pCmdUI);
}


void CEditorView::OnUpdateCameraScale(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnUpdateCameraScale(pCmdUI);
}


void CEditorView::OnAddCylniderPop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAddCylinder();
}


void CEditorView::OnAddCone()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAddCone();
}


void CEditorView::OnAddConePop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAddCone();
}


void CEditorView::OnAddParticlePop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAddParticle();
}


void CEditorView::OnAddBillboardPop()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pFrame = (CMainFrame*)GetParent();
	pFrame->OnAddBillboard();
}
