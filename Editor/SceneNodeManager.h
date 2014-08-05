
#ifndef SCENENODEMANAGER_H
#define SCENENODEMANAGER_H

#include "Irrlicht.h"
#include <vector>
#include <string>

namespace irr
{
	namespace scene
	{
		//节点类型
		enum NodeType
		{
			TYPE_CUBE = 0,
			TYPE_SPHERE = 1,
			TYPE_ROOT = 2,
			TYPE_NONE = 3,
			TYPE_SKY_BOX = 4,
			TYPE_LIGHT = 5,
			TYPE_BILL = 6,
			TYPE_CYLINDER = 7,
			TYPE_CONE = 8,
			TYPE_PARTICLE = 9,
			TYPE_BILLTEXT=10
		};
		//动画类型
		enum AnimType
		{
			AT_NONE,
			AT_STRAIGHT,
			AT_CYCLE,
			AT_ROTATION
		};
		//节点属性结构体，用于传递节点属性
		struct NodeInfo{
			s32 nodeID;
			std::string name;
			NodeType nodeType;
			bool enableLight;
			core::vector3df position;
			core::vector3df size;
			core::vector3df rotate;
			video::SColorf color;
			f32 radius;
		};
		struct AnimateInfo{
			int animateType;
			core::vector3df center;
			f32 radius;
			f32 speed;
			core::vector3df startPoint;
			core::vector3df endPoint;
			u32 timeForWay;
			bool pingpang;
			core::vector3df rotationSpeed;
			AnimateInfo(){
				animateType = AT_NONE;
				center = core::vector3df(0.f,0.f,30.f);
				radius=20.f;	
				speed=0.001f;
				startPoint = core::vector3df(100,0,60);
				endPoint = core::vector3df(-100,0,60);
				timeForWay = 3500;
				pingpang = false;
				rotationSpeed = core::vector3df(0,1,0);
			}
		};
		class ISceneNodeManager
		{
		public:
			static const char * NodeName[];
			ISceneNode* m_pSkyBox;
		private:
			/*成员变量*/
			IrrlichtDevice* m_pDevice;
            ISceneManager* m_pSceneManager;
			video::IVideoDriver* m_pDriver;
			ICameraSceneNode* m_pCamera;
			ILightSceneNode* m_pLight;
			IMeshSceneNode* m_pSphere;
			IMeshSceneNode* m_pCube;
			IMeshSceneNode* m_pCylinder;
			IMeshSceneNode* m_pCone;
			IBillboardSceneNode * m_pBillboard;
			IBillboardTextSceneNode * m_pBillboardText;
			IParticleSystemSceneNode* m_pParticleSystem;
			std::vector<IMeshSceneNode*> m_vGeoVector;
			std::vector<ILightSceneNode*> m_vLightVector;
			ISceneNode* m_pCurrentNode;//表示当前被选择的节点
			ISceneNodeManager();
			static ISceneNodeManager* instance;
			std::vector<std::string> m_vTexturePos;

			//
			std::vector<AnimateInfo> m_vAnimateInfo;
			//

			static int numOfNodes;

			//单例状态变量
			bool m_bInitial;
		public:
			
			static void initial(IrrlichtDevice * device);
			//添加一个获取device的接口getDevice
			IrrlichtDevice * getDevice()const;

			ISceneManager* getSceneManager();

			static ISceneNodeManager* ISceneNodeManager::getInstance();

			bool isSelectable(ISceneNode*)const;//判断一个节点是否可被选择

			std::vector<IMeshSceneNode *> getMeshSceneNodes()const;

			std::vector<ILightSceneNode *> getLightSceneNodes()const;

			bool setCurrentNode(ISceneNode *);

			void setCurrentValue(NodeInfo node);//修改当前节点的属性

			void deleteCurrentNode();

			/*把一个MeshSceneNode添加进当前的节点环境*/
			bool addMeshSceneNode(ISceneNode*);

			/*用于从irr文件中添加节点*/
			bool addMeshSceneNodeFromIrr(IMeshSceneNode * node,int num,NodeType type);

			bool addMeshSceneNodeFromIrr(IAnimatedMeshSceneNode * node,int num,NodeType type);

			bool addMeshSceneNodeFromIrr(const core::vector3df &position,const core::vector3df &rotation,const core::vector3df &scale,int num,NodeType type);

			bool addSpecialSceneNodeFromIrr(ISceneNode* node,int num,NodeType type);

			bool addSkyBoxFromIrr(ISceneNode *);

			/*为导出代码记录纹理 */
			void setTexture(int num,std::string pos);

			/*为导出代码获得纹理 */
			std::string getTexture(int num);

			//modified
			bool setAnimatedValue(AnimateInfo node);

			AnimateInfo getAnimatedValue()const;

			AnimateInfo getAnimatedValue(irr::s32 id)const;

			bool hasAnimator();

			static void clear();

			void removeAnimator();
			//

			/*动画：绕圆圈飞*/
			void setCycleFly(
				const core::vector3df& center=core::vector3df(0.f,0.f,30.f),
				f32 radius=20.f, 
				f32 speed=0.001f);


			/*动画：绕直线飞*/
			void setStraightFly(
				const core::vector3df& startPoint = core::vector3df(100,0,60),
				const core::vector3df& endPoint = core::vector3df(-100,0,60), 
				u32 timeForWay = 3500, 
				bool loop=false, 
				bool pingpong = false);
			
			void setRotation(const core::vector3df& speed = core::vector3df(0,1,0));

			bool addCurrentNode(int type);

			ICameraSceneNode* addCamera(core::vector3df &target);

			NodeInfo getCurrentNodeInfo()const;

			ISceneNode * getCurrentNode()const;

            IMeshSceneNode* addSphere (
				f32 radius=5.0f,
				s32 polyCount=16,
				ISceneNode *parent=0,
				s32 id=-1,
				const core::vector3df &position=core::vector3df(0, 0, 0),
				const core::vector3df &rotation=core::vector3df(0, 0, 0),
				const core::vector3df &scale=core::vector3df(1.0f, 1.0f, 1.0f));

			IMeshSceneNode* addCube(f32 size=10.0f, 
				ISceneNode *parent=0, 
				s32 id = -1,
				const core::vector3df &position=core::vector3df(0, 0, 0),
				const core::vector3df &rotation=core::vector3df(0, 0, 0),
				const core::vector3df &scale=core::vector3df(1.0f, 1.0f, 1.0f));

			ILightSceneNode* addLight(
				ISceneNode *parent=0,
				const core::vector3df &position=core::vector3df(0, 0, 0), 
				video::SColorf color=video::SColorf(1.0f, 1.0f, 1.0f),
				f32 radius=100.0f,
				s32 id=-1);

			IAnimatedMeshSceneNode* addSydney();
	
			IMeshSceneNode* addSkyBox(std::string up ="media/irrlicht2_up.jpg" ,
									std::string down = "media/irrlicht2_dn.jpg",
									std::string left = "media/irrlicht2_lf.jpg",
									std::string right = "media/irrlicht2_rt.jpg",
									std::string front = "media/irrlicht2_ft.jpg",
									std::string back = "media/irrlicht2_bk.jpg");

			IBillboardSceneNode* addBillboard (
				ISceneNode *parent=0, 
				const core::dimension2d< f32 > &size=core::dimension2d<f32>(50, 50), 
				const core::vector3df &position=core::vector3df(0, 0, 0), 
				s32 id=-1, video::SColor colorTop=0xFFFFFFFF, 
				video::SColor colorBottom=0xFFFFFFFF
				);

			IBillboardTextSceneNode* addBillboardText (
				gui::IGUIFont *font = 0, const wchar_t *text =  L"Hello Irr", 
				ISceneNode *parent = 0, const core::dimension2d< f32 > &size=core::dimension2d<f32>(50, 50), 
				const core::vector3df &position=core::vector3df(0, 0, 0), 
				s32 id=-1, video::SColor colorTop=0xFFFFFFFF, 
				video::SColor colorBottom=0xFFFFFFFF);

			IParticleSystemSceneNode* ISceneNodeManager::addParticleSystem (
				bool withDefaultEmitter=true, ISceneNode* parent=0, s32 id=-1,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			IMeshSceneNode* addCylinder(
				f32 radius = 10.f, 
				f32 length = 10.f,
				u32 tesselation = 10,
				const video::SColor& color=video::SColor(0xffffffff),
				bool closeTop=true, f32 oblique=0.f);

			IMeshSceneNode* addCone(
				f32 radius = 10.f, f32 length = 10.f, u32 tesselation = 10,
				const video::SColor& colorTop=0xffffffff,
				const video::SColor& colorBottom=0xffffffff,
				f32 oblique=0.f);

			
			/*IMeshSceneNode* addArrowMesh(
				const u32 tesselationCylinder,
				const u32 tesselationCone, const f32 height,
				const f32 cylinderHeight, const f32 width0,
				const f32 width1, const video::SColor vtxColor0,
				const video::SColor vtxColor1);*/
		};

	};
};

#endif

