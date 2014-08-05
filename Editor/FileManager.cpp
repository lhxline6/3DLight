#include "stdafx.h"
#include "FileManager.h"
#include "SceneNodeManager.h"
#include <fstream>
#include <time.h>


using namespace irr;
using namespace scene;
using namespace std;

FileManager* FileManager::instance = NULL;

void FileManager::initial(IrrlichtDevice * device)
{
	if(!instance)
		instance = new FileManager();
	instance->m_pDevice = device;
	instance->m_pSceneManager =  device->getSceneManager();
	instance->m_bInitial = true;
}



FileManager* FileManager::getInstance()
{
	if(instance==NULL)
		instance = new FileManager();
	if(instance->m_bInitial==false)
		return NULL;
	return instance;
}

FileManager::FileManager()
{
	m_bInitial = false;
}

bool FileManager::loadScene(io::path fliename)
{
	//成功返回true

	//修改
		int numofnodes = 0;
	//

	core::array<scene::ISceneNode *> nodes;
	bool flag = m_pSceneManager->loadScene(fliename);
	m_pSceneManager->getSceneNodesFromType(ESNT_ANY,nodes);
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	for (u32 i=0; i < nodes.size(); ++i) {
		if(nodes[i]->getType() == ESNT_SKY_BOX){
			nodes[i]->setID(numofnodes);
		}
		if(nodes[i]->getType() == ESNT_CUBE){
			nodes[i]->setID(numofnodes);
			snmgr->addMeshSceneNodeFromIrr((IMeshSceneNode*)nodes[i],numofnodes++,TYPE_CUBE);
		}
		if(nodes[i]->getType() == ESNT_SPHERE){
			nodes[i]->setID(numofnodes);
			snmgr->addMeshSceneNodeFromIrr((IMeshSceneNode*)nodes[i],numofnodes++,TYPE_SPHERE);
		}
		if(nodes[i]->getType() == ESNT_ANIMATED_MESH){
			nodes[i]->setID(numofnodes);
			snmgr->addMeshSceneNodeFromIrr((IAnimatedMeshSceneNode*)nodes[i],numofnodes++,TYPE_CUBE);
		}
		if(nodes[i]->getType() == ESNT_MESH){
			string tmpMeshName = nodes[i]->getName();
			if(tmpMeshName=="testCone"){
				nodes[i]->setID(numofnodes);
				snmgr->addMeshSceneNodeFromIrr(nodes[i]->getPosition(),nodes[i]->getRotation(),nodes[i]->getScale(),++numofnodes, TYPE_CONE);
				nodes[i]->remove();
			}
			if(tmpMeshName=="testCylinder"){
				nodes[i]->setID(numofnodes);
				snmgr->addMeshSceneNodeFromIrr(nodes[i]->getPosition(),nodes[i]->getRotation(),nodes[i]->getScale(),++numofnodes, TYPE_CYLINDER);
				nodes[i]->remove();
			}
		}
		if(nodes[i]->getType() == ESNT_PARTICLE_SYSTEM){
			nodes[i]->setID(numofnodes);
			snmgr->addSpecialSceneNodeFromIrr(nodes[i],numofnodes++,TYPE_PARTICLE);
		}
	}


	return flag;
}

bool FileManager::saveScene(io::path fliename)
{
	//成功返回true
	return m_pSceneManager->saveScene(fliename);
}


string fisrtPara = "#include <irrlicht.h>\nusing namespace irr;\nusing namespace core;\nusing namespace scene;\nusing namespace video;\nusing namespace io;\nusing namespace gui;\n\n\
#ifdef _IRR_WINDOWS_\n#pragma comment(lib, \"Irrlicht.lib\")\n#pragma comment(linker, \"/subsystem:windows /ENTRY:mainCRTStartup\")\n#endif\n\nscene::ISceneNodeAnimator* anim;\nIAnimatedMeshSceneNode* node;\nIMeshSceneNode* cube;\nIMeshSceneNode* sphere;\nIMeshSceneNode* geoNode;\nITexture * texture;\nconst IGeometryCreator* gc;\nscene::IMesh *mesh;\n\IParticleSystemSceneNode* particleSystem;\n\
\nint main(){\n\tIrrlichtDevice *device =createDevice( irr::video::EDT_DIRECT3D9, dimension2d<u32>(800, 600), 16,false, false, false, 0);\n\
\tIVideoDriver* driver = device->getVideoDriver();\n\tISceneManager* smgr = device->getSceneManager();\n\tIGUIEnvironment* guienv = device->getGUIEnvironment();\n\n";

string lastPara = "\n\tsmgr->addSkyBoxSceneNode(driver->getTexture(\"media/irrlicht2_up.jpg\"),\n\t\tdriver->getTexture(\"media/irrlicht2_dn.jpg\"),\n\t\tdriver->getTexture(\"media/irrlicht2_lf.jpg\"),\n\t\tdriver->getTexture(\"media/irrlicht2_rt.jpg\"),\n\t\tdriver->getTexture(\"media/irrlicht2_ft.jpg\"),\n\t\tdriver->getTexture(\"media/irrlicht2_bk.jpg\"));\n\
\n\twhile(device->run()){\n\t\tdriver->beginScene(true, true, SColor(255,100,101,140));\n\t\tsmgr->drawAll();\n\t\tguienv->drawAll();\n\t\tdriver->endScene();\n\t}\n\tdevice->drop();\n\treturn 0;\n}";



void FileManager::exportCode(string path)
{
	ofstream fout(path.c_str());
	core::array<scene::ISceneNode *> nodes;
	m_pSceneManager->getSceneNodesFromType(ESNT_ANY,nodes);
	/*
	if(m_pSceneManager->getSceneNodeFromName("testBillBoardText")){
		fout<<"fefefefefa"<<endl;
	}
	*/
	if(nodes.size() == 0) return;
	fout<<fisrtPara;
	ISceneNodeManager * snmgr = ISceneNodeManager::getInstance();
	std::vector<IMeshSceneNode*> meshNodes = snmgr->getMeshSceneNodes();
	for (u32 i=0; i < nodes.size(); ++i) {
		if(nodes[i]->getType() == ESNT_CUBE){
			fout<<"\n\tcube = smgr->addCubeSceneNode(10.0f,0,-1,core::vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"),core::vector3df("<<nodes[i]->getRotation().X<<","<<nodes[i]->getRotation().Y<<","<<nodes[i]->getRotation().Z<<"),core::vector3df("<<nodes[i]->getScale().X<<","<<nodes[i]->getScale().Y<<","<<nodes[i]->getScale().Z<<"));"<<endl;
			if(snmgr->getTexture(nodes[i]->getID())!="null"){
				string tmp = snmgr->getTexture(nodes[i]->getID());
				for(int i=0; i <  tmp.size(); ++i){
					if(tmp[i] == '\\')
						tmp[i] = '/';
				}
				fout<<"\ttexture = driver->getTexture(\""<< tmp<<"\");\n\tcube->setMaterialTexture(0,texture);\n\tcube->setMaterialFlag(video::EMF_LIGHTING, false); ";
			}
			irr::scene::AnimateInfo tmp = snmgr->getAnimatedValue(nodes[i]->getID());
			if(tmp.animateType == AT_CYCLE){
				fout<<"\tanim =smgr->createFlyCircleAnimator(core::vector3df("<<tmp.center.X<<","<<tmp.center.Y<<","<<tmp.center.Z<<"),"<<tmp.radius<<","<<tmp.speed<<");\n\tif (anim){\n\t\tcube->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType ==AT_STRAIGHT ){
				fout<<"\tanim = smgr->createFlyStraightAnimator(core::vector3df("<<tmp.startPoint.X<<","<<tmp.startPoint.Y<<","<<tmp.startPoint.Z<<"),core::vector3df("<<tmp.endPoint.X<<","<<tmp.endPoint.Y<<","<<tmp.endPoint.Z<<"),"<<tmp.timeForWay<<",true);\n\tif (anim){\n\t\tcube->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_ROTATION){
				fout<<"\tanim = smgr->createRotationAnimator(core::vector3df("<<tmp.rotationSpeed.X<<","<<tmp.rotationSpeed.Y<<","<<tmp.rotationSpeed.Z<<"));\n\tif (anim){\n\t\tcube->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
		}
		if(nodes[i]->getType() == ESNT_SPHERE){
			fout<<"\n\tsphere = smgr->addSphereSceneNode(5.0f,16,0,-1,core::vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"),core::vector3df("<<nodes[i]->getRotation().X<<","<<nodes[i]->getRotation().Y<<","<<nodes[i]->getRotation().Z<<"),core::vector3df("<<nodes[i]->getScale().X<<","<<nodes[i]->getScale().Y<<","<<nodes[i]->getScale().Z<<"));"<<endl;
			if(snmgr->getTexture(nodes[i]->getID())!="null"){
				string tmp = snmgr->getTexture(nodes[i]->getID());
				for(int i=0; i <  tmp.size(); ++i){
					if(tmp[i] == '\\')
						tmp[i] = '/';
				}
				fout<<"\ttexture = driver->getTexture(\""<< tmp<<"\");\n\tsphere->setMaterialTexture(0,texture);\n\tsphere->setMaterialFlag(video::EMF_LIGHTING, false); ";
			}
			irr::scene::AnimateInfo tmp = snmgr->getAnimatedValue(nodes[i]->getID());
			if(tmp.animateType == AT_CYCLE){
				fout<<"\tanim =smgr->createFlyCircleAnimator(core::vector3df("<<tmp.center.X<<","<<tmp.center.Y<<","<<tmp.center.Z<<"),"<<tmp.radius<<","<<tmp.speed<<");\n\tif (anim){\n\t\tsphere->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_STRAIGHT){
				fout<<"\tanim = smgr->createFlyStraightAnimator(core::vector3df("<<tmp.startPoint.X<<","<<tmp.startPoint.Y<<","<<tmp.startPoint.Z<<"),core::vector3df("<<tmp.endPoint.X<<","<<tmp.endPoint.Y<<","<<tmp.endPoint.Z<<"),"<<tmp.timeForWay<<",true);\n\tif (anim){\n\t\tsphere->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_ROTATION){
				fout<<"\tanim = smgr->createRotationAnimator(core::vector3df("<<tmp.rotationSpeed.X<<","<<tmp.rotationSpeed.Y<<","<<tmp.rotationSpeed.Z<<"));\n\tif (anim){\n\t\tsphere ->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
		}
		if(nodes[i]->getType() == ESNT_LIGHT){
			fout<<"\tsmgr->addLightSceneNode(0,vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"),SColorf(1.0f, 1.0f, 1.0f),100.0f,-1);"<<endl;
		}
		if(nodes[i]->getType() == ESNT_ANIMATED_MESH){
			fout<<"\n\tIAnimatedMesh* mesh = smgr->getMesh(\"media/sydney.md2\");\n\tif(mesh){\n\t\tnode = smgr->addAnimatedMeshSceneNode(mesh);\n\t\tif (node){\n\
				 node->setMaterialFlag(video::EMF_LIGHTING, false);\n\t\t\tnode->setMD2Animation(scene::EMAT_STAND);\n\t\t\tnode->setMaterialTexture(0,driver->getTexture(\"media/sydney.bmp\") );\n\t\t}\n\t};\n";
			
			irr::scene::AnimateInfo tmp = snmgr->getAnimatedValue(nodes[i]->getID());
			if(tmp.animateType == AT_CYCLE){
				fout<<"\tanim =smgr->createFlyCircleAnimator(core::vector3df("<<tmp.center.X<<","<<tmp.center.Y<<","<<tmp.center.Z<<"),"<<tmp.radius<<","<<tmp.speed<<");\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType ==AT_STRAIGHT ){
				fout<<"\tanim = smgr->createFlyStraightAnimator(core::vector3df("<<tmp.startPoint.X<<","<<tmp.startPoint.Y<<","<<tmp.startPoint.Z<<"),core::vector3df("<<tmp.endPoint.X<<","<<tmp.endPoint.Y<<","<<tmp.endPoint.Z<<"),"<<tmp.timeForWay<<",true);\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_ROTATION){
				fout<<"\tanim = smgr->createRotationAnimator(core::vector3df("<<tmp.rotationSpeed.X<<","<<tmp.rotationSpeed.Y<<","<<tmp.rotationSpeed.Z<<"));\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
		}
		
		string tmpMeshName = nodes[i]->getName();
		if(tmpMeshName=="testCone" || tmpMeshName=="testCylinder"){
			if(tmpMeshName=="testCone"){
					fout<<"\n\tgc = smgr->getGeometryCreator();\n\tmesh = gc->createConeMesh(10,10,10);\n\tmesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);\n\tgeoNode = smgr->addMeshSceneNode(mesh);\n\tgeoNode->setPosition(core::vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"));geoNode->setRotation(core::vector3df("<<nodes[i]->getRotation().X<<","<<nodes[i]->getRotation().Y<<","<<nodes[i]->getRotation().Z<<"));\n\tgeoNode->setScale(core::vector3df("<<nodes[i]->getScale().X<<","<<nodes[i]->getScale().Y<<","<<nodes[i]->getScale().Z<<"));"<<endl;
			}
			else if(tmpMeshName=="testCylinder"){
				fout<<"\n\tgc = smgr->getGeometryCreator();\n\tmesh = gc->createCylinderMesh(10,10,10);\n\tmesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);\n\tgeoNode = smgr->addMeshSceneNode(mesh);\n\tgeoNode->setPosition(core::vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"));geoNode->setRotation(core::vector3df("<<nodes[i]->getRotation().X<<","<<nodes[i]->getRotation().Y<<","<<nodes[i]->getRotation().Z<<"));\n\tgeoNode->setScale(core::vector3df("<<nodes[i]->getScale().X<<","<<nodes[i]->getScale().Y<<","<<nodes[i]->getScale().Z<<"));"<<endl;
			}

			if(snmgr->getTexture(nodes[i]->getID())!="null"){
				string tmp = snmgr->getTexture(nodes[i]->getID());
				for(int i=0; i <  tmp.size(); ++i){
					if(tmp[i] == '\\')
						tmp[i] = '/';
				}
				fout<<"\ttexture = driver->getTexture(\""<< tmp<<"\");\n\tnode->setMaterialTexture(0,texture);\n\tnode->setMaterialFlag(video::EMF_LIGHTING, false); ";
			}

			
			irr::scene::AnimateInfo tmp = snmgr->getAnimatedValue(nodes[i]->getID());
			if(tmp.animateType == AT_CYCLE){
				fout<<"\tanim =smgr->createFlyCircleAnimator(core::vector3df("<<tmp.center.X<<","<<tmp.center.Y<<","<<tmp.center.Z<<"),"<<tmp.radius<<","<<tmp.speed<<");\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_STRAIGHT){
				fout<<"\tanim = smgr->createFlyStraightAnimator(core::vector3df("<<tmp.startPoint.X<<","<<tmp.startPoint.Y<<","<<tmp.startPoint.Z<<"),core::vector3df("<<tmp.endPoint.X<<","<<tmp.endPoint.Y<<","<<tmp.endPoint.Z<<"),"<<tmp.timeForWay<<",true);\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_ROTATION){
				fout<<"\tanim = smgr->createRotationAnimator(core::vector3df("<<tmp.rotationSpeed.X<<","<<tmp.rotationSpeed.Y<<","<<tmp.rotationSpeed.Z<<"));\n\tif (anim){\n\t\tgeoNode->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			
		}	
		if(nodes[i]->getType() == ESNT_PARTICLE_SYSTEM){
			fout<<"\n\tparticleSystem = smgr->addParticleSystemSceneNode(true,0,-1,core::vector3df("<<nodes[i]->getPosition().X<<","<<nodes[i]->getPosition().Y<<","<<nodes[i]->getPosition().Z<<"),core::vector3df("<<nodes[i]->getRotation().X<<","<<nodes[i]->getRotation().Y<<","<<nodes[i]->getRotation().Z<<"),core::vector3df("<<nodes[i]->getScale().X<<","<<nodes[i]->getScale().Y<<","<<nodes[i]->getScale().Z<<"));"<<endl;
			if(snmgr->getTexture(nodes[i]->getID())!="null"){
				string tmp = snmgr->getTexture(nodes[i]->getID());
				for(int i=0; i <  tmp.size(); ++i){
					if(tmp[i] == '\\')
						tmp[i] = '/';
				}
				fout<<"\ttexture = driver->getTexture(\""<< tmp<<"\");\n\tparticleSystem->setMaterialTexture(0,texture);\n\tparticleSystem->setMaterialFlag(video::EMF_LIGHTING, false); ";
			}
			irr::scene::AnimateInfo tmp = snmgr->getAnimatedValue(nodes[i]->getID());
			if(tmp.animateType == AT_CYCLE){
				fout<<"\tanim =smgr->createFlyCircleAnimator(core::vector3df("<<tmp.center.X<<","<<tmp.center.Y<<","<<tmp.center.Z<<"),"<<tmp.radius<<","<<tmp.speed<<");\n\tif (anim){\n\t\tparticleSystem->addAnimator(anim);\n\t\tparticleSystem->drop();\n\t}";
			}
			else if(tmp.animateType ==AT_STRAIGHT ){
				fout<<"\tanim = smgr->createFlyStraightAnimator(core::vector3df("<<tmp.startPoint.X<<","<<tmp.startPoint.Y<<","<<tmp.startPoint.Z<<"),core::vector3df("<<tmp.endPoint.X<<","<<tmp.endPoint.Y<<","<<tmp.endPoint.Z<<"),"<<tmp.timeForWay<<",true);\n\tif (anim){\n\t\tparticleSystem->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
			else if(tmp.animateType == AT_ROTATION){
				fout<<"\tanim = smgr->createRotationAnimator(core::vector3df("<<tmp.rotationSpeed.X<<","<<tmp.rotationSpeed.Y<<","<<tmp.rotationSpeed.Z<<"));\n\tif (anim){\n\t\tparticleSystem->addAnimator(anim);\n\t\tanim->drop();\n\t}";
			}
		}
	}

	scene::ICameraSceneNode *cam =  m_pSceneManager->getActiveCamera();
	fout<<"\n\tsmgr->addCameraSceneNode(0, vector3df("<<cam->getPosition().X<<","<<cam->getPosition().Y<<","<<cam->getPosition().Z<<"), vector3df("<<cam->getTarget().X<<","<<cam->getTarget().Y<<","<<cam->getTarget().Z<<"));\n\n";
	fout<<lastPara;
	fout.close();
}
