#include "stdafx.h"
#include "EditCamera.h"
#include "SceneNodeManager.h"
#include "UIListener.h"

using namespace irr;
using namespace scene;

ICameraAnimator::ICameraAnimator(IrrlichtDevice * device,ISceneCollisionManager* CollMan,
								 ICameraSceneNode * camera,float move_speed,
								 float rotate_speed,float sight_distance)
{
	m_pDevice = device;
	m_pCamera = camera;
	m_iCollsionManager =  CollMan;
	camera->bindTargetAndRotation(true);
	//鼠标动作
	m_bFirstViewControl = true;
	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_iLPressDown = 0;
	m_iRPressDown = 0;
	m_iLPressUp = 0;
	m_iRPressUp = 0;
	//键盘动作
	m_bUp = false;
	m_bDown = false;
	m_bLeft = false;
	m_bRight = false;
	//参数变量
	m_fMoveSpeed = move_speed;//这个速度是<距离/ms>
	m_fRotateSpeed = rotate_speed;//这个速度是<角度/鼠标相对距离>
	m_fSightDist = sight_distance;
	m_fScaleSpeed;
	m_iLastTime = 0;
	m_cursorState = ViewControl;
	m_keyState = PosControl;
	m_nodeState = Move;
	m_bEnterNodeControl = true;

	m_pCursor = device->getCursorControl();
	m_pCursor->grab();
	m_fMaxVerticalAngle = 89.f;
}

ICameraAnimator::~ICameraAnimator()
{
}

ICameraAnimator::NodeState ICameraAnimator::getNodeState()const
{
	return m_nodeState;
}
void ICameraAnimator::setNodeState(NodeState state)
{
	m_nodeState = state;
}

void ICameraAnimator::cursorViewControl(u32 timeDiff)
{
	/*如果当前帧刚好按下鼠标按钮且点中物体，就切换cursor状态*/
	if(m_iLPressDown||m_iRPressDown)
	{
		// Tracks the current intersection point with the level or a mesh
		core::vector3df intersection;
		// Used to show with triangle has been hit
		core::triangle3df hitTriangle;


		m_iCollsionManager = m_pDevice->getSceneManager()->getSceneCollisionManager();
		core::line3df ray = 
			m_iCollsionManager->getRayFromScreenCoordinates(m_v2diCurPos,m_pCamera);

		m_iSelectedNode = 
			m_iCollsionManager->getSceneNodeAndCollisionPointFromRay(ray,
			intersection,
			hitTriangle,0);
		if(m_iSelectedNode)
		{
			video::IVideoDriver * driver = m_pDevice->getVideoDriver();
			driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));
		}
		
		
		ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
		if(m_iSelectedNode)
		{
			snmgr->setCurrentNode(m_iSelectedNode);
			if(m_dsAxis!=NONE)
			{
				m_cursorState = NodeControl;
			}
		}
	}

	core::vector3df position =  m_pCamera->getAbsolutePosition();
	core::vector3df up = m_pCamera->getUpVector();
	core::vector3df target = m_pCamera->getTarget();
	core::vector3df dis = target - position;
	core::vector3df rotation= dis.getHorizontalAngle();
	//如果鼠标左键松开，就确定当前旋转
	if(m_iLPressUp)
	{
		m_v3dfRawRotation = rotation;
	}
	//如果鼠标左键没有按下，就不变换
	if(!m_bLButtonDown)
		return ;
	if(m_bFirstViewControl)
	{
		m_v3dfRawRotation = rotation;
		m_bFirstViewControl =false;
	}
	//计算鼠标变换差
	core::vector2df shift = m_v2dfCurPos - m_v2dfPrePos;
	rotation.X = m_v3dfRawRotation.X+shift.Y*m_fRotateSpeed;
	rotation.Y = m_v3dfRawRotation.Y+shift.X*m_fRotateSpeed;
	if(rotation.X<0)
		rotation.X += 360.f;
	else if(rotation.X>360.f)
		rotation.X -= 360.f;
	if(rotation.Y<0)
		rotation.Y += 360.f;
	else if(rotation.Y>360.f)
		rotation.Y -= 360.f;
	//限制纵坐标
	if(rotation.X > m_fMaxVerticalAngle*2 &&
		rotation.X<360.f - m_fMaxVerticalAngle)
		rotation.X = 360.f-m_fMaxVerticalAngle;
	else if(rotation.X > m_fMaxVerticalAngle &&
		rotation.X<360.f - m_fMaxVerticalAngle)
		rotation.X = m_fMaxVerticalAngle;
	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(rotation.X,rotation.Y,0));
	dis.set(0,0,1.0f);
	mat.transformVect(dis);
	dis.setLength(m_fSightDist);
	target = position + dis;
	m_pCamera->setTarget(target);
	if(target==core::vector3df(0,0,0))
		target = core::vector3df(0,0,0);
}

void ICameraAnimator::cursorNodeControl(u32 timeDiff)
{
	if(!m_bLButtonDown)
	{
		m_cursorState = ViewControl;
		m_bEnterNodeControl = true;
		UIListener * ul = UIListener::getInstance();
		ul->sendMessage(UIListener::PropertyPane);
		return ;
	}
	
	if(m_bEnterNodeControl)
	{
		m_v3dfInitObjPos = m_iSelectedNode->getAbsolutePosition();
		m_v3dfInitObjRot = m_iSelectedNode->getRotation();
		m_v3dfInitObjScale = m_iSelectedNode->getScale();
		m_v2dfPrePos = m_v2dfCurPos;
		m_v2diPrePos = m_v2diCurPos;
		m_fObjSpeed = (m_v3dfInitObjPos - m_pCamera->getPosition()).getLength();
		m_bEnterNodeControl = false;
	}
	//计算鼠标变换差
	core::vector2df shift = m_v2dfCurPos - m_v2dfPrePos;
	core::vector3df pos = m_v3dfInitObjPos;
	core::vector3df rot = m_v3dfInitObjRot;
	core::vector3df scale = m_v3dfInitObjScale;
	core::line3df ray(m_iCollsionManager->getRayFromScreenCoordinates(m_v2diCurPos, m_pCamera));
	core::line3df pre_ray(m_iCollsionManager->getRayFromScreenCoordinates(m_v2diPrePos, m_pCamera));
	core::vector3df intersection;
	core::vector3df cam_dir = m_pCamera->getTarget() - m_pCamera->getAbsolutePosition();
	core::plane3df const plane(m_pCamera->getTarget(),cam_dir);
	if(m_nodeState == Move){
		if(m_dsAxis==X){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.X = dir3d.X>0?1.5f:-1.5f;
			pos += core::vector3df(dir3d.X*shift.getLength()*m_fObjSpeed,0,0);
		}
		else if(m_dsAxis==Y){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Y = dir3d.Y>0?1.5f:-1.5f;
			pos += core::vector3df(0,dir3d.Y*shift.getLength()*m_fObjSpeed,0);
		}
		else if(m_dsAxis==Z){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Z = dir3d.Z>0?1.5f:-1.5f;
			pos += core::vector3df(0,0,dir3d.Z*shift.getLength()*m_fObjSpeed);
		}
		else
			return ;
		m_iSelectedNode->setPosition(pos);
	}
	else if(m_nodeState == Rotate){
		if(m_dsAxis==X){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.X = dir3d.X>0?1.5f:-1.5f;
			rot += core::vector3df(dir3d.X*shift.getLength()*m_fRotateSpeed,0,0);
		}
		else if(m_dsAxis==Y){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Y = dir3d.Y>0?1.5f:-1.5f;
			rot += core::vector3df(0,dir3d.Y*shift.getLength()*m_fRotateSpeed,0);
		}
		else if(m_dsAxis==Z){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Z = dir3d.Z>0?1.5f:-1.5f;
			rot += core::vector3df(0,0,dir3d.Z*shift.getLength()*m_fRotateSpeed);
		}
		m_iSelectedNode->setRotation(rot);
	}
	else if(m_nodeState == Scale){
		core::vector3df dist = m_iSelectedNode->getAbsolutePosition() - m_pCamera->getAbsolutePosition();
		m_fScaleSpeed = dist.getLength()/12;
		if(m_dsAxis==X){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.X = dir3d.X>0?1.5f:-1.5f;
			scale += core::vector3df(dir3d.X*shift.getLength()*m_fScaleSpeed,0,0);
		}
		else if(m_dsAxis==Y){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Y = dir3d.Y>0?1.5f:-1.5f;
			scale += core::vector3df(0,dir3d.Y*shift.getLength()*m_fScaleSpeed,0);
		}
		else if(m_dsAxis==Z){
			plane.getIntersectionWithLine(ray.start, ray.getVector(),intersection);
			core::vector3df pre_inter;
			plane.getIntersectionWithLine(pre_ray.start, pre_ray.getVector(),pre_inter);
			core::vector3df dir3d = intersection - pre_inter;
			dir3d.Z = dir3d.Z>0?1.5f:-1.5f;
			scale += core::vector3df(0,0,dir3d.Z*shift.getLength()*m_fScaleSpeed);
		}
		m_iSelectedNode->setScale(scale);
	}
}

void ICameraAnimator::keyPosControl(u32 timeDiff)
{
	//如果没有按键，就不更新
	if( !m_bUp && !m_bDown && !m_bLeft && !m_bRight)
		return ;
	core::vector3df direction(0.f,0.f,0.f);
	core::vector3df position =  m_pCamera->getAbsolutePosition();
	core::vector3df target = m_pCamera->getTarget();
	core::vector3df dis = target - position;
	core::vector3df left = dis.crossProduct(m_pCamera->getUpVector());
	left.normalize();
	dis.normalize();
	if(m_bUp)
	{
		direction += dis*m_fMoveSpeed*timeDiff;
	}
	if(m_bDown)
	{
		direction -= dis*m_fMoveSpeed*timeDiff;
	}
	if(m_bLeft)
	{
		direction += left*m_fMoveSpeed*timeDiff;
	}
	if(m_bRight)
	{
		direction -= left*m_fMoveSpeed*timeDiff;
	}
	position += direction;
	dis.setLength(m_fSightDist);
	target = position+dis;
	m_pCamera->setPosition(position);
	m_pCamera->setTarget(target);
}

void ICameraAnimator::animateNode(ISceneNode * node,u32 timeMS)
{
	int timeDiff = timeMS - m_iLastTime;
	m_iLastTime = timeMS;
	updateControl();//更新输入
	adjustRatio(node);//整理显示比例
	if(m_cursorState == ViewControl)
		cursorViewControl(timeDiff);

	if(m_cursorState == NodeControl)
	{
		cursorNodeControl(timeDiff);
	}

	if(m_keyState == PosControl)
		keyPosControl(timeDiff);
	
}

ISceneNodeAnimator * ICameraAnimator::createClone(ISceneNode * node,ISceneManager * newManager)
{
	return (new ICameraAnimator(m_pDevice,m_iCollsionManager,m_pCamera));
}

bool ICameraAnimator::isEventReceiverEnabled()const
{
	return true;
}
void ICameraAnimator::updateControl()
{
	m_pCursor = m_pDevice->getCursorControl();
	core::vector2di pix = m_pCursor->getPosition();
	pix.Y += 23;
	irr::video::IVideoDriver * driver = m_pDevice->getVideoDriver();
	core::dimension2du client = driver->getScreenSize();
	if(pix.X<0||pix.Y<0||pix.X>client.Width||pix.Y>client.Height)
		return ;
	core::vector2df pos = m_pCursor->getRelativePosition();
	m_bUp = KeyDown(VK_UP);
	m_bDown = KeyDown(VK_DOWN);
	m_bLeft = KeyDown(VK_LEFT);
	m_bRight = KeyDown(VK_RIGHT);
	m_bW = KeyDown('A');
	m_bS = KeyDown(83);
	m_bD = KeyDown(68);

	/*更新拖拽轴状态*/
	if(m_bS)
	{
		m_dsAxis = X;
	}
	else if(m_bD)
	{
		m_dsAxis = Y;
	}
	else if(m_bW)
	{
		m_dsAxis = Z;
	}
	else
		m_dsAxis = NONE;
	
	m_v2diCurPos = pix;
	m_v2dfCurPos = m_pCursor->getRelativePosition();
	//检测鼠标按键动作
	bool curLButtonDown = KeyDown(VK_LBUTTON);
	bool curRButtonDown = KeyDown(VK_RBUTTON);
	bool l_pressed,r_pressed,l_released,r_released;
	if( (m_bLButtonDown==false)&&(curLButtonDown==true) )
	{
		l_pressed = true;
		l_released = false;
		m_bLButtonDown = true;
	}
	else if( (m_bLButtonDown==true)&&(curLButtonDown==false) )
	{
		l_released = true;
		l_pressed = false;
		m_bLButtonDown = false;
	}
	else
	{
		l_released = false;
		l_pressed = false;
	}

	if( (m_bRButtonDown==false)&&(curRButtonDown==true) )
	{
		r_pressed = true;
		r_released = false;
		m_bRButtonDown = true;
	}
	else if( (m_bRButtonDown==true)&&(curRButtonDown==false) )
	{
		r_released = true;
		r_pressed = false;
		m_bRButtonDown = false;
	}
	else
	{
		r_released = false;
		r_pressed = false;
	}
	m_iLPressUp = l_released?1:0;
	m_iRPressUp = r_released?1:0;
	m_iLPressDown = l_pressed?1:0;
	m_iRPressDown = r_pressed?1:0;
	if(l_pressed)
		m_v2dfPrePos = m_v2dfCurPos;
}

void ICameraAnimator::adjustRatio(ISceneNode * node)
{
	if(m_pDevice)
	{
		video::IVideoDriver * driver = m_pDevice->getVideoDriver();
		core::dimension2du screen = driver->getScreenSize();
		float ratio =  	screen.Width*1.0/screen.Height;
		m_pCamera->setAspectRatio(ratio);
	}
}

/*=========================
*				相关外部接口
*========================*/


ICameraSceneNode * irr::scene::addEditCameraSceneNode(
	IrrlichtDevice * device,ISceneNode * parent,
			core::vector3df start_position,core::vector3df target,
			float move_speed,float rotate_speed,float sight_distance)
{
	ISceneManager * smgr = device->getSceneManager();
	ICameraSceneNode * camera = smgr->addCameraSceneNode(parent,start_position,target);
	ISceneCollisionManager* CollMan      = smgr->getSceneCollisionManager();
	ICameraAnimator * animator = new ICameraAnimator(device,CollMan,
		camera,move_speed,rotate_speed,sight_distance);
	camera->addAnimator(animator);
	animator->drop();
	device->getSceneManager()->setActiveCamera(camera);
	return camera;
}

void irr::scene::setEditCameraState(ICameraSceneNode * pCamera,ICameraAnimator::NodeState state)
{
	core::list<ISceneNodeAnimator *> ary = pCamera->getAnimators();
	if(ary.size()>1)
		return ;
	ICameraAnimator * pa = (ICameraAnimator*)*ary.begin();
	pa->setNodeState(state);
}

ICameraAnimator::NodeState irr::scene::getEditCameraState(ICameraSceneNode * pCamera)
{
	core::list<ISceneNodeAnimator *> ary = pCamera->getAnimators();
	if(ary.size()>1)
		return (ICameraAnimator::NodeState)-1;
	ICameraAnimator * pa = (ICameraAnimator*)*ary.begin();
	return pa->getNodeState();
}

void irr::scene::setEditCameraTarget(ICameraSceneNode * pCamera,ISceneNode * node)
{
	if(pCamera==NULL)
		return;
	ICameraAnimator * an = (ICameraAnimator*)(*pCamera->getAnimators().begin());
	float distance = an->m_fSightDist;
	pCamera->setTarget(node->getAbsolutePosition());
	core::vector3df dir = node->getAbsolutePosition() - pCamera->getAbsolutePosition();
	dir.setLength(distance);
	pCamera->setTarget(pCamera->getAbsolutePosition()+dir);
	an->m_v3dfRawRotation = pCamera->getRotation();
}

irr::scene::ICameraAnimator::DragState irr::scene::getEditCameraDragDir(ICameraSceneNode * pCamera)
{
	ICameraAnimator * an = (ICameraAnimator*)(*pCamera->getAnimators().begin());
	return an->m_dsAxis;
}
