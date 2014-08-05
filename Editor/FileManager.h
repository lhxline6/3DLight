#ifndef MANAGESCENE_H
#define MANAGESCENE_H
#include "Irrlicht.h"
#include "driverChoice.h"
#include <string>

namespace irr
{
	namespace scene
	{
		class FileManager
		{
		private:
			/*��Ա����*/
			IrrlichtDevice* m_pDevice;
			ISceneManager* m_pSceneManager;
			FileManager();
			static FileManager* instance;
			bool m_bInitial;
		public:
			/*���캯������������*/
			static void initial(IrrlichtDevice * device);
			FileManager* getSceneManager();
			static FileManager* FileManager::getInstance();

		public:
			/*��ȡ����*/
			bool loadScene(io::path fliename);
			/*���泡��*/
			bool saveScene(io::path fliename);
			/*�������*/
			void exportCode(std::string path);
		};
	};
};



#endif
