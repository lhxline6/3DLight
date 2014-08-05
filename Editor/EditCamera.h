#ifndef EDITCAMERA_H
#define EDITCAMERA_H
#include "Irrlicht.h"
#include "stdafx.h"

#define KeyDown(x) (GetAsyncKeyState(x)&0x8000?true:false)
#define KeyUp(x) (GetAsyncKeyState(x)&0x8000?false:true)

namespace irr
{
	namespace scene
	{
		class ICameraAnimator :public ISceneNodeAnimator
		{
		public:
			/*描述鼠标所处于的状态，默认是ViewControl，表示移动视角*/
			enum CursorState
			{
				ViewControl,
				NodeControl
			};
			/*描述键盘所处的状态，默认是PosControl，表示移动摄像机*/
			enum KeyState
			{
				PosControl
			};
			/*描述节点所处的状态*/
			enum NodeState
			{
				Move,
				Rotate,
				Scale
			};
			/*描述是否激活了拖拽控制*/
			enum DragState
			{
				X,
				Y,
				Z,
				NONE
			};
		public:
			/*环境变量*/
			IrrlichtDevice * m_pDevice;
			ICameraSceneNode * m_pCamera;
			gui::ICursorControl * m_pCursor;
			/*状态变量*/
			DragState m_dsAxis;
				//鼠标坐标都是相对坐标，即0.f~1.f的浮点数
			core::vector2df m_v2dfPrePos;//上一次鼠标坐标
			core::vector2df m_v2dfCurPos;//鼠标当前坐标

			core::vector2di m_v2diCurPos;//鼠标当前绝对坐标（相对于窗口）
			core::vector2di m_v2diPrePos;//鼠标上次绝对坐标（相对于窗口）
				//鼠标动作
			bool m_bFirstViewControl;
			bool m_bLButtonDown;
			bool m_bRButtonDown;
			int m_iLPressDown;
			int m_iRPressDown;
			int m_iLPressUp;
			int m_iRPressUp;
				//刷新选项
			int m_iLastTime;
				//旋转动作
			core::vector3df m_v3dfRawRotation;
				//方向按键
			bool m_bUp;
			bool m_bDown;
			bool m_bLeft;
			bool m_bRight;
			bool m_bW;
			bool m_bS;
			bool m_bD;
			/*参数变量*/
			CursorState m_cursorState;
			KeyState m_keyState;
			NodeState m_nodeState;
			float m_fMoveSpeed;
			float m_fRotateSpeed;
			float m_fSightDist;
			float m_fMaxVerticalAngle;


			irr::core::vector3df m_v3dfInitObjPos;      // where ray from camera to object intersected screen 
			irr::core::vector3df m_v3dfInitObjRot;
			irr::core::vector3df m_v3dfInitObjScale;
			float m_fObjSpeed;
			ISceneNode* m_iSelectedNode;
			bool m_bEnterNodeControl;
			float m_fNodeRotateSpeed;
			float m_fScaleSpeed;
			ISceneCollisionManager* m_iCollsionManager;

		public:
			/*构造、析构函数*/
			ICameraAnimator(IrrlichtDevice * device,ISceneCollisionManager* CollMan,
				ICameraSceneNode * camera,float move_speed=0.1f,
				float rotate_speed=150.f,float sight_distance=70.f);
			~ICameraAnimator();

			NodeState getNodeState()const;
			void setNodeState(NodeState state);
		private:
			/*=============================
			* animateNode中的辅助函数，用于控制分工;
			*moveLeft/Right/Up/Down控制摄像头移动
			==============================*/
			
			//用于更新输入系统
			void updateControl();
			
			//用于更新摄像机视角
			void adjustRatio(ISceneNode * node);

			//*cursorViewControl用于根据鼠标动作调整视角
			void cursorViewControl(u32 timeDiff);
			
			//*cursorNodeControl用于对节点进行控制
			void cursorNodeControl(u32 timeDiff);

			//*keyPosControl用于根据键盘调整摄像头位置
			void keyPosControl(u32 timeDiff);

		public:
			/*接口函数*/
			void animateNode(ISceneNode * node,u32 timeMS);
			ISceneNodeAnimator * createClone(ISceneNode * node,ISceneManager * newManager);
			bool isEventReceiverEnabled()const;
		};

		//外部接口，提供一个EditCamera的入口
		ICameraSceneNode * addEditCameraSceneNode(
			IrrlichtDevice * device,ISceneNode * parent=NULL,
			core::vector3df start_position = core::vector3df(30,30,30),
			core::vector3df target = core::vector3df(0,0,0),
			float move_speed=0.1f,float rotate_speed=150.f,float sight_distance=300.f);

		//设置摄像机的节点编辑模式
		void setEditCameraState(ICameraSceneNode * pCamera,ICameraAnimator::NodeState state);
		ICameraAnimator::NodeState getEditCameraState(ICameraSceneNode * pCamera);
		//读取摄像机的轴模式
		ICameraAnimator::DragState getEditCameraDragDir(ICameraSceneNode * pCamera);
		//设置摄像机的方向
		void setEditCameraTarget(ICameraSceneNode * pCamera,ISceneNode * node);
	};
};

#endif

