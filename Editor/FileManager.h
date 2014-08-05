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
			/*成员变量*/
			IrrlichtDevice* m_pDevice;
			ISceneManager* m_pSceneManager;
			FileManager();
			static FileManager* instance;
			bool m_bInitial;
		public:
			/*构造函数与析构函数*/
			static void initial(IrrlichtDevice * device);
			FileManager* getSceneManager();
			static FileManager* FileManager::getInstance();

		public:
			/*读取场景*/
			bool loadScene(io::path fliename);
			/*保存场景*/
			bool saveScene(io::path fliename);
			/*保存代码*/
			void exportCode(std::string path);
		};
	};
};



#endif
