#include "stdafx.h"
#include "SceneNodeManager.h"
#include "UIListener.h"
#include <fstream>

using namespace irr;
using namespace scene;
using namespace std;

ISceneNodeManager* ISceneNodeManager::instance = NULL;

int ISceneNodeManager::numOfNodes = 0;

const char * ISceneNodeManager::NodeName[] = 
{
	"testCube",
	"testSphere",
	"root",
	"none",
	"skybox",
	"testLight",
	"testBillBoard",
	"testCylinder",
	"testCone",
	"testParticle",
	"testBillBoardText"
};

ISceneNodeManager* ISceneNodeManager::getInstance()
{
	if(instance==NULL)
		instance = new ISceneNodeManager();
	if(instance->m_bInitial==false)
		return NULL;
	return instance;
}

ISceneNodeManager::ISceneNodeManager()
{
	m_bInitial = false;
	m_pSkyBox = NULL;
};

ISceneManager* ISceneNodeManager::getSceneManager(){
	return m_pSceneManager; 
}

ICameraSceneNode* ISceneNodeManager::addCamera(core::vector3df &target=core::vector3df(0,0,0)){
	m_pCamera = m_pSceneManager->addCameraSceneNode();
	m_pCamera->setTarget(core::vector3df(0,0,0));
	return m_pCamera;
}

vector<IMeshSceneNode *> ISceneNodeManager::getMeshSceneNodes()const
{
	return m_vGeoVector;
}

vector<ILightSceneNode *> ISceneNodeManager::getLightSceneNodes()const
{
	return m_vLightVector;
}

bool ISceneNodeManager::isSelectable(ISceneNode* pCurNode)const
{
	vector<IMeshSceneNode*>::iterator iter;
	for(int i=0;i<m_vGeoVector.size();i++)
	{
		if(m_vGeoVector[i]==pCurNode)
			return true;
	}
	return false;
}


bool ISceneNodeManager::setCurrentNode(ISceneNode * pCurNode)
{
	vector<IMeshSceneNode*>::iterator iter;
	int i;
	m_pCurrentNode = NULL;

	do
	{
		for ( i = 0 ; i < m_vGeoVector.size() ; i++ )
		{
			m_vGeoVector[i]->setMaterialFlag(video::EMF_LIGHTING,true);
			if(m_vGeoVector[i] == pCurNode){
				m_pCurrentNode = m_vGeoVector[i];	
			}
		}
		if(m_pCurrentNode)
		{
		
			m_pCurrentNode->setMaterialFlag(video::EMF_LIGHTING,false);
			break;
		}
		for(i=0;i<m_vLightVector.size();i++)
		{
			if(m_vLightVector[i] == pCurNode)
			{
				m_pCurrentNode = pCurNode;
				break;
			}
		}
	}while(false);
	if(m_pCurrentNode==NULL)
		return false;
	UIListener * ul = UIListener::getInstance();
	ul->sendMessage(UIListener::PropertyPane);
	return true;
}


void ISceneNodeManager::deleteCurrentNode()
{
	vector<IMeshSceneNode *>::iterator iter;
	for(iter=m_vGeoVector.begin();iter!=m_vGeoVector.end();)
	{
		if((*iter)==m_pCurrentNode)
		{
			(*iter)->remove();
			m_vGeoVector.erase(iter++);
			break;
		}
		else ++iter;
	}
	m_pCurrentNode = NULL;
}


NodeInfo ISceneNodeManager::getCurrentNodeInfo()const
{
	NodeInfo info;
	ISceneManager * smgr = m_pSceneManager;
	if(m_pCurrentNode==NULL)
	{
		info.name = smgr->getRootSceneNode()->getName();
		info.nodeID = smgr->getRootSceneNode()->getID();
		info.nodeType = TYPE_ROOT;
	}
	else
	{
		info.name = m_pCurrentNode->getName();
		info.nodeID = m_pCurrentNode->getID();
		int esnt = m_pCurrentNode->getType();
		switch(esnt)
		{
		case ESNT_CUBE:
			info.nodeType = TYPE_CUBE;break;
		case ESNT_ANIMATED_MESH:
				info.nodeType = TYPE_CUBE;break;
		case ESNT_SPHERE:
			info.nodeType = TYPE_SPHERE;break;
		case ESNT_SKY_BOX:
			info.nodeType = TYPE_SKY_BOX;break;
		case ESNT_LIGHT:
			info.nodeType = TYPE_LIGHT;break;
		case ESNT_BILLBOARD:
			info.nodeType = TYPE_BILL;break;
		case ESNT_PARTICLE_SYSTEM:
			info.nodeType = TYPE_PARTICLE;break;
		default:
			if(info.name=="testCone")
				info.nodeType = TYPE_CONE;
			else if(info.name=="testCylinder")
				info.nodeType = TYPE_CYLINDER;
			else if(info.name=="testBillBoardText")
				info.nodeType = TYPE_BILLTEXT;
			else
				info.nodeType = TYPE_NONE;
			break;
		}
		video::SMaterial material = m_pCurrentNode->getMaterial(0);
		info.enableLight = material.Lighting;
		info.position = m_pCurrentNode->getPosition();
		info.size = m_pCurrentNode->getScale();
		info.rotate = m_pCurrentNode->getRotation();

		if(info.nodeType==TYPE_LIGHT)
		{
			ILightSceneNode * light_node = (ILightSceneNode*)m_pCurrentNode;
			video::SLight light = light_node->getLightData(); 
			info.radius = light.Radius;
			info.color = light.AmbientColor;
		}
	}
	return info;
}

ISceneNode * ISceneNodeManager::getCurrentNode()const
{
	return m_pCurrentNode;
}


IMeshSceneNode* ISceneNodeManager::addSphere (
				f32 radius, 
				s32 polyCount, 
				ISceneNode *parent, 
				s32 id, 
				const core::vector3df &position,
				const core::vector3df &rotation,
				const core::vector3df &scale)
{

	m_pSphere = m_pSceneManager->addSphereSceneNode(radius, polyCount, parent, numOfNodes++, position,rotation,scale );

	if(m_pSphere)
	{
		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(m_pSphere->getMesh(),m_pSphere);
		m_pSphere->setTriangleSelector(selector);
		selector->drop();
		m_pSphere->setName(NodeName[TYPE_SPHERE]);
		m_vGeoVector.push_back(m_pSphere);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}
	return m_pSphere;
}


IMeshSceneNode* ISceneNodeManager::addCube(f32 size, ISceneNode *parent, s32 id, const core::vector3df &position, const core::vector3df &rotation, const core::vector3df &scale){
	m_pCube = m_pSceneManager->addCubeSceneNode (size, parent,numOfNodes++, position,  rotation, scale);

	if(m_pCube)
	{
		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(m_pCube->getMesh(),m_pCube);
		m_pCube->setTriangleSelector(selector);
		selector->drop();
		m_pCube->setName(NodeName[TYPE_CUBE]);
		m_vGeoVector.push_back(m_pCube);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}

	return m_pCube;
}

ILightSceneNode* ISceneNodeManager::addLight(
				ISceneNode *parent, 
				const core::vector3df &position, 
				video::SColorf color, 
				f32 radius, s32 id){
	m_pLight = m_pSceneManager-> addLightSceneNode(parent,position,color,radius,numOfNodes++);
	
	if(m_pLight){
		m_pLight->setName(NodeName[TYPE_LIGHT]);
		m_vLightVector.push_back(m_pLight);
		m_vTexturePos.push_back("null");
		m_vAnimateInfo.push_back(AnimateInfo());
	}
	return m_pLight;
}

IAnimatedMeshSceneNode* ISceneNodeManager::addSydney(){
	IAnimatedMesh* mesh = m_pSceneManager->getMesh("media/sydney.md2");
	if (!mesh){
		m_pDevice->drop();
		return NULL;
	}
	IAnimatedMeshSceneNode* node = m_pSceneManager->addAnimatedMeshSceneNode(mesh,0,numOfNodes++);
	if (node){
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0,m_pDriver->getTexture("media/sydney.bmp") );

		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		node->setTriangleSelector(selector);
		selector->drop();
		node->setName(NodeName[TYPE_CUBE]);
		m_vGeoVector.push_back( (IMeshSceneNode*)node);
		m_vTexturePos.push_back("D:/Editor/Editor/media/sydney.bmp");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	
	}
	return node;
}


IMeshSceneNode* ISceneNodeManager::addSkyBox(string up,string down,string left,string right,string front,string back){
	m_pSceneManager->addSkyBoxSceneNode(
		m_pDriver->getTexture(up.c_str()),
		m_pDriver->getTexture(down.c_str()),
		m_pDriver->getTexture(left.c_str()),
		m_pDriver->getTexture(right.c_str()),
		m_pDriver->getTexture(front.c_str()),
		m_pDriver->getTexture(back.c_str()));
	return m_pCube;

}

/*

先不用它了，用下面的addBillboardText;
IBillboardSceneNode* ISceneNodeManager::addBillboard (ISceneNode *parent, const core::dimension2d< f32 > &size, const core::vector3df &position, s32 id, video::SColor colorTop, video::SColor colorBottom){
	m_pBillboardText =  m_pSceneManager->addBillboardTextSceneNode (0,L"HElloe", parent,size, position,numOfNodes++,colorTop,colorBottom);
	m_pBillboard =  m_pSceneManager->addBillboardSceneNode (m_pBillboardText,size, position,numOfNodes++, colorTop, colorBottom);
	

	if(m_pBillboard)
	{
		ITriangleSelector * selector = m_pSceneManager->createTriangleSelectorFromBoundingBox(m_pBillboard);
		m_pBillboard->setTriangleSelector(selector);
		selector->drop();
		m_pBillboard->setName(NodeName[TYPE_BILL]);
		m_vGeoVector.push_back((IMeshSceneNode*)m_pBillboard);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}
	return m_pBillboard; 
}
*/


IBillboardTextSceneNode* ISceneNodeManager::addBillboardText (gui::IGUIFont *font , const wchar_t *text, ISceneNode *parent, const core::dimension2d< f32 > &size, const core::vector3df &position, s32 id, video::SColor colorTop, video::SColor colorBottom){
	m_pBillboardText =  m_pSceneManager->addBillboardTextSceneNode (font,text, parent,size, position,numOfNodes++,colorTop,colorBottom);
//	m_pBillboard =  m_pSceneManager->addBillboardSceneNode (m_pBillboardText,size, position,id, colorTop, colorBottom);

	

	if(m_pBillboardText)
	{
		ITriangleSelector * selector = m_pSceneManager->createTriangleSelectorFromBoundingBox(m_pBillboardText);
		m_pBillboardText->setTriangleSelector(selector);
		selector->drop();
		m_pBillboardText->setName(NodeName[TYPE_BILLTEXT]);
		m_vGeoVector.push_back((IMeshSceneNode*)m_pBillboardText);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}
	return m_pBillboardText; 
}

IParticleSystemSceneNode* ISceneNodeManager::addParticleSystem (bool withDefaultEmitter, ISceneNode* parent, s32 id,const core::vector3df& position,const core::vector3df& rotation,const core::vector3df& scale ){
	m_pParticleSystem = m_pSceneManager->addParticleSystemSceneNode(withDefaultEmitter,parent,numOfNodes++,position,rotation,scale);

	if(m_pParticleSystem)
	{
		ITriangleSelector * selector = m_pSceneManager->createTriangleSelectorFromBoundingBox(m_pParticleSystem);
		m_pParticleSystem->setTriangleSelector(selector);
		selector->drop();
		m_pParticleSystem->setName(NodeName[TYPE_PARTICLE]);
		m_vGeoVector.push_back((IMeshSceneNode*)m_pParticleSystem);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}
	return m_pParticleSystem;	 
}



void ISceneNodeManager::initial(IrrlichtDevice * device)
{
	if(!instance)
		instance = new ISceneNodeManager();
	instance->m_pDevice = device;
	instance->m_pSceneManager =  device->getSceneManager();
	instance->m_pDriver = device->getVideoDriver();
	instance->m_pCurrentNode = NULL;
	instance->m_bInitial = true;
	numOfNodes = 0;


}

IrrlichtDevice * ISceneNodeManager::getDevice()const
{
	return m_pDevice;
}

void ISceneNodeManager::setCurrentValue(NodeInfo node)
{
	if(m_pCurrentNode)
	{
		m_pCurrentNode->setPosition(node.position);
		m_pCurrentNode->setScale(node.size);
		m_pCurrentNode->setRotation(node.rotate);
		m_pCurrentNode->setName(node.name.c_str());
		m_pCurrentNode->setMaterialFlag(video::EMF_LIGHTING,node.enableLight);
		if(node.nodeType==TYPE_LIGHT)
		{
			ILightSceneNode * light_node = (ILightSceneNode*)m_pCurrentNode;
			video::SLight light = light_node->getLightData();
			light.Radius = node.radius;
			light.AmbientColor = node.color;
			light.DiffuseColor = node.color;
			light.SpecularColor = node.color;
			light_node->setLightData(light);
		}
	}
}


bool ISceneNodeManager::addMeshSceneNode(ISceneNode * node)
{
	m_vGeoVector.push_back(((IMeshSceneNode*)node));
	return true;
}


bool ISceneNodeManager::addMeshSceneNodeFromIrr(IMeshSceneNode * node,int num,NodeType type)
{
	if(node)
	{

		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		
		node->setTriangleSelector(selector);
		selector->drop();
		node->setName(NodeName[type]);
		AnimateInfo Anode;
		
		core::list<scene::ISceneNodeAnimator*> animators = node->getAnimators();
		core::list<scene::ISceneNodeAnimator*>::Iterator it;
		it=animators.begin();
		while(it!=animators.end()){
			node->addAnimator(*it);
			Anode.animateType = (*it)->getType();
			it++;
		}

		m_vGeoVector.push_back(((IMeshSceneNode*)node));
		numOfNodes = num;
		m_vTexturePos.push_back("null");
		m_vAnimateInfo.push_back(Anode);
		//
	}
	return true;
}

bool ISceneNodeManager::addSpecialSceneNodeFromIrr(ISceneNode* node,int num,NodeType type)
{
	if(node)
	{
		ITriangleSelector * selector = m_pSceneManager->createTriangleSelectorFromBoundingBox(node);
		node->setTriangleSelector(selector);
		selector->drop();
		
		node->setName(NodeName[type]);
		AnimateInfo Anode;
		
		core::list<scene::ISceneNodeAnimator*> animators = node->getAnimators();
		core::list<scene::ISceneNodeAnimator*>::Iterator it;
		it=animators.begin();
		while(it!=animators.end()){
			node->addAnimator(*it);
			Anode.animateType = (*it)->getType();
			it++;
		}

		m_vGeoVector.push_back(((IMeshSceneNode*)node));
		numOfNodes = num;
		m_vTexturePos.push_back("null");
		m_vAnimateInfo.push_back(Anode);
		//
	}
	return true;
}

//type
bool ISceneNodeManager::addMeshSceneNodeFromIrr(IAnimatedMeshSceneNode * node,int num,NodeType type)
{
	if(node)
	{

		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		
		node->setTriangleSelector(selector);
		selector->drop();
		node->setName(NodeName[type]);
		
		AnimateInfo Anode;
		core::list<scene::ISceneNodeAnimator*> animators = node->getAnimators();
		core::list<scene::ISceneNodeAnimator*>::Iterator it;
		it=animators.begin();
		while(it!=animators.end()){
			node->addAnimator(*it);
			Anode.animateType = (*it)->getType();
			it++;
		}


		m_vGeoVector.push_back(((IMeshSceneNode*)node));
		numOfNodes = num;
		m_vTexturePos.push_back("null");
		m_vAnimateInfo.push_back(Anode);
		//
	}
	return true;
}

bool ISceneNodeManager::addSkyBoxFromIrr(ISceneNode * node){
	m_pSkyBox = node;
	return true;
}

bool ISceneNodeManager::addMeshSceneNodeFromIrr(const core::vector3df &position,const core::vector3df &rotation,const core::vector3df &scale,int num,NodeType type)
{
	const IGeometryCreator* gc = m_pSceneManager->getGeometryCreator();
	scene::IMesh *mesh;
	if(type == TYPE_CONE)
		mesh = gc->createConeMesh(10,10,10);
	else if(type == TYPE_CYLINDER)
		mesh = gc->createCylinderMesh(10,10,10);
	else
		return false;
	mesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);    
	IMeshSceneNode* node = m_pSceneManager->addMeshSceneNode(mesh);
	
	if(node)
	{
		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		node->setTriangleSelector(selector);
		selector->drop();
		node->setPosition(position);
		node->setScale(scale);
		node->setName(NodeName[type]);
		node->setID(num-1);
		numOfNodes = num;

		AnimateInfo Anode;

		core::list<scene::ISceneNodeAnimator*> animators = node->getAnimators();
		core::list<scene::ISceneNodeAnimator*>::Iterator it;
		it=animators.begin();
		while(it!=animators.end()){
			node->addAnimator(*it);
			Anode.animateType = (*it)->getType();
			it++;
		}

		m_vGeoVector.push_back((IMeshSceneNode*)node);
		m_vTexturePos.push_back("null");
		m_vAnimateInfo.push_back(Anode);
	}
	return true;
}

void ISceneNodeManager::setTexture(int num,string pos)
{
	if(num<0) return;
	m_vTexturePos[num] = pos;
}

string ISceneNodeManager::getTexture(int num)
{
	if(num >= m_vTexturePos.size())return "null";
	return m_vTexturePos[num] ;
}


//modefied
//animateType =  1 means CycleFly, animateType = 2 means StraightFly
bool ISceneNodeManager::setAnimatedValue(AnimateInfo node)
{
	if(m_pCurrentNode==NULL)
		return false;
	if(node.animateType == AT_CYCLE){
		m_vAnimateInfo[m_pCurrentNode->getID()] = node;
		setCycleFly(node.center,node.radius,node.speed);
		return true;
	}
	else if(node.animateType == AT_STRAIGHT){
		m_vAnimateInfo[m_pCurrentNode->getID()] = node;
		setStraightFly(node.startPoint,node.endPoint,node.timeForWay,false,node.pingpang);
		return true;
	}
	else if(node.animateType == AT_ROTATION){
		m_vAnimateInfo[m_pCurrentNode->getID()] = node;
		setRotation(node.rotationSpeed);
		return true;
	}
	else false;
}


bool ISceneNodeManager::hasAnimator(){
	if(m_pCurrentNode==NULL)
		return false;
	if( m_pCurrentNode->getAnimators().size()==0)
		return false;
	else 
		return true;
}

void ISceneNodeManager::removeAnimator()
{
	if(m_pCurrentNode==NULL)
		return;
	m_pCurrentNode->removeAnimators();
}

AnimateInfo ISceneNodeManager::getAnimatedValue()const{
	//m_vAnimateInfo[m_pCurrentNode->getID()].center;
	return m_vAnimateInfo[m_pCurrentNode->getID()];
}

AnimateInfo ISceneNodeManager::getAnimatedValue(irr::s32 nodeid)const{
	//m_vAnimateInfo[m_pCurrentNode->getID()].center;
	return m_vAnimateInfo[nodeid];
}

//


void ISceneNodeManager::setCycleFly(const core::vector3df& center,f32 radius, f32 speed)
{
	scene::ISceneNodeAnimator* anim =m_pSceneManager->createFlyCircleAnimator(center,radius,speed);
	if (anim)
	{
		if(m_pCurrentNode==NULL) return;
		m_pCurrentNode->removeAnimators();
		m_pCurrentNode->addAnimator(anim);
		AnimateInfo info;
		info.animateType = AT_CYCLE;
		info.center = center;
		info.radius = radius;
		info.speed = speed;
		m_vAnimateInfo[m_pCurrentNode->getID()] = info;
		anim->drop();
	}
}



void ISceneNodeManager::setStraightFly(const core::vector3df& startPoint,
			const core::vector3df& endPoint, u32 timeForWay, bool loop, bool pingpong)
{
	scene::ISceneNodeAnimator* anim = 
		m_pSceneManager->createFlyStraightAnimator(startPoint,endPoint, timeForWay, true,pingpong);
		if (anim)
		{
			if(m_pCurrentNode==NULL) return;
			m_pCurrentNode->removeAnimators();
			m_pCurrentNode->addAnimator(anim);
			AnimateInfo info;
			info.animateType = AT_STRAIGHT;
			info.startPoint = startPoint;
			info.endPoint = endPoint;
			info.timeForWay = timeForWay;
			info.pingpang = pingpong;
			m_vAnimateInfo[m_pCurrentNode->getID()] = info;
			anim->drop();
		}
}

void ISceneNodeManager::clear()
{
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	if(snmgr==NULL)
		return ;
	ISceneManager * smgr = snmgr->m_pDevice->getSceneManager();
	smgr->clear();
	snmgr->m_vAnimateInfo.clear();
	snmgr->m_vGeoVector.clear();
	snmgr->m_vLightVector.clear();
	snmgr->m_vTexturePos.clear();
	snmgr->m_pSkyBox = NULL;
}

void ISceneNodeManager::setRotation(const core::vector3df& speed)
{
	scene::ISceneNodeAnimator* anim = 
		m_pSceneManager->createRotationAnimator(speed);
	if (anim)
	{
		if(m_pCurrentNode==NULL) return;
		m_pCurrentNode->addAnimator(anim);
		AnimateInfo info;
		info.animateType = AT_ROTATION;
		info.rotationSpeed = speed;
		m_vAnimateInfo[m_pCurrentNode->getID()] = info;
		anim->drop();
	}
}


IMeshSceneNode* ISceneNodeManager::addCylinder(f32 radius, f32 length,u32 tesselation,const video::SColor& color,bool closeTop, f32 oblique){
	
	const IGeometryCreator* gc = m_pSceneManager->getGeometryCreator();
	scene::IMesh *mesh = gc->createCylinderMesh(radius,length,tesselation,color,closeTop,oblique);
	mesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);    
	scene::IMeshSceneNode* node = m_pSceneManager->addMeshSceneNode(mesh);
	

	
	if(node)
	{
		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		node->setTriangleSelector(selector);
		selector->drop();
		node->setName(NodeName[TYPE_CYLINDER]);
		node->setID(numOfNodes++);
		m_vGeoVector.push_back(node);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}

	return node;
}

//ofstream fout("ff.cpp");
IMeshSceneNode* ISceneNodeManager::addCone(f32 radius, f32 length, u32 tesselation,
					const video::SColor& colorTop, 
					const video::SColor& colorBottom,
					f32 oblique){	
	const IGeometryCreator* gc = m_pSceneManager->getGeometryCreator();
	scene::IMesh *mesh = gc->createConeMesh(radius,length,tesselation);
	mesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);    
	scene::IMeshSceneNode* node = m_pSceneManager->addMeshSceneNode(mesh);
	
	
	if(node)
	{
		scene::ITriangleSelector * selector = 
			m_pSceneManager->createTriangleSelector(node->getMesh(),node);
		node->setTriangleSelector(selector);
		selector->drop();
		node->setName(NodeName[TYPE_CONE]);
		//fout<<(node->getName())<<endl;
		node->setID(numOfNodes++);
		m_vGeoVector.push_back(node);
		m_vTexturePos.push_back("null");
		AnimateInfo Anode;
		m_vAnimateInfo.push_back(Anode);
	}
	return node;
}

