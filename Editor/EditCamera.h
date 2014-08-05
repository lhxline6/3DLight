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
			/*������������ڵ�״̬��Ĭ����ViewControl����ʾ�ƶ��ӽ�*/
			enum CursorState
			{
				ViewControl,
				NodeControl
			};
			/*��������������״̬��Ĭ����PosControl����ʾ�ƶ������*/
			enum KeyState
			{
				PosControl
			};
			/*�����ڵ�������״̬*/
			enum NodeState
			{
				Move,
				Rotate,
				Scale
			};
			/*�����Ƿ񼤻�����ק����*/
			enum DragState
			{
				X,
				Y,
				Z,
				NONE
			};
		public:
			/*��������*/
			IrrlichtDevice * m_pDevice;
			ICameraSceneNode * m_pCamera;
			gui::ICursorControl * m_pCursor;
			/*״̬����*/
			DragState m_dsAxis;
				//������궼��������꣬��0.f~1.f�ĸ�����
			core::vector2df m_v2dfPrePos;//��һ���������
			core::vector2df m_v2dfCurPos;//��굱ǰ����

			core::vector2di m_v2diCurPos;//��굱ǰ�������꣨����ڴ��ڣ�
			core::vector2di m_v2diPrePos;//����ϴξ������꣨����ڴ��ڣ�
				//��궯��
			bool m_bFirstViewControl;
			bool m_bLButtonDown;
			bool m_bRButtonDown;
			int m_iLPressDown;
			int m_iRPressDown;
			int m_iLPressUp;
			int m_iRPressUp;
				//ˢ��ѡ��
			int m_iLastTime;
				//��ת����
			core::vector3df m_v3dfRawRotation;
				//���򰴼�
			bool m_bUp;
			bool m_bDown;
			bool m_bLeft;
			bool m_bRight;
			bool m_bW;
			bool m_bS;
			bool m_bD;
			/*��������*/
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
			/*���졢��������*/
			ICameraAnimator(IrrlichtDevice * device,ISceneCollisionManager* CollMan,
				ICameraSceneNode * camera,float move_speed=0.1f,
				float rotate_speed=150.f,float sight_distance=70.f);
			~ICameraAnimator();

			NodeState getNodeState()const;
			void setNodeState(NodeState state);
		private:
			/*=============================
			* animateNode�еĸ������������ڿ��Ʒֹ�;
			*moveLeft/Right/Up/Down��������ͷ�ƶ�
			==============================*/
			
			//���ڸ�������ϵͳ
			void updateControl();
			
			//���ڸ���������ӽ�
			void adjustRatio(ISceneNode * node);

			//*cursorViewControl���ڸ�����궯�������ӽ�
			void cursorViewControl(u32 timeDiff);
			
			//*cursorNodeControl���ڶԽڵ���п���
			void cursorNodeControl(u32 timeDiff);

			//*keyPosControl���ڸ��ݼ��̵�������ͷλ��
			void keyPosControl(u32 timeDiff);

		public:
			/*�ӿں���*/
			void animateNode(ISceneNode * node,u32 timeMS);
			ISceneNodeAnimator * createClone(ISceneNode * node,ISceneManager * newManager);
			bool isEventReceiverEnabled()const;
		};

		//�ⲿ�ӿڣ��ṩһ��EditCamera�����
		ICameraSceneNode * addEditCameraSceneNode(
			IrrlichtDevice * device,ISceneNode * parent=NULL,
			core::vector3df start_position = core::vector3df(30,30,30),
			core::vector3df target = core::vector3df(0,0,0),
			float move_speed=0.1f,float rotate_speed=150.f,float sight_distance=300.f);

		//����������Ľڵ�༭ģʽ
		void setEditCameraState(ICameraSceneNode * pCamera,ICameraAnimator::NodeState state);
		ICameraAnimator::NodeState getEditCameraState(ICameraSceneNode * pCamera);
		//��ȡ���������ģʽ
		ICameraAnimator::DragState getEditCameraDragDir(ICameraSceneNode * pCamera);
		//����������ķ���
		void setEditCameraTarget(ICameraSceneNode * pCamera,ISceneNode * node);
	};
};

#endif

