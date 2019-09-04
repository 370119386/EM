#include <assert.h>
#include "PluginManager.h"
#include "./SDK/IModule.h"
#include "./SDK/IPlugin.h"
#include "./Common/MemObjectTracer.h"
#include "EMDynLib.h"

using namespace EM;

PluginManager::PluginManager()
{
	EM::MemObjectTracer::Instance();
	mPluginNameDic.clear();
	mPluginLibDic.clear();
	mPluginHandleDic.clear();
}

PluginManager::~PluginManager()
{
	EM::MemObjectTracer::Instance()->Release();
}

void EM::PluginManager::LoadPlugin()
{

}

bool EM::PluginManager::Awake()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->Awake();
		}
	}
	return true;
}

bool EM::PluginManager::Init()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->Init();
		}
	}
	return true;
}

bool EM::PluginManager::AfterInit()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->AfterInit();
		}
	}
	return true;
}

bool EM::PluginManager::CheckConfig()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->CheckConfig();
		}
	}
	return true;
}

bool EM::PluginManager::ReadyExecute()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->ReadyExecute();
		}
	}
	return true;
}

bool EM::PluginManager::Execute()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->Execute();
		}
	}
	return true;
}

bool EM::PluginManager::BeforeShut()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->BeforeShut();
		}
	}
	return true;
}

bool EM::PluginManager::Shut()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->Shut();
		}
	}
	return true;
}

bool EM::PluginManager::Finalize()
{
	for (PluginInstanceMap::const_iterator itr = mPluginHandleDic.begin(); itr != mPluginHandleDic.end(); ++itr)
	{
		if (nullptr != itr->second)
		{
			itr->second->Finalize();
		}
	}

	for (PluginNameMap::const_iterator itr = mPluginNameDic.begin(); itr != mPluginNameDic.end(); ++itr)
	{
		UnLoadPluginLibrary(itr->first);
	}

	mPluginHandleDic.clear();
	mPluginNameDic.clear();

	return true;
}

void EM::PluginManager::Registered(IPlugin* plugin)
{
	const std::string& strPluginName = plugin->GetPluginName();
	if (mPluginHandleDic.find(strPluginName) == mPluginHandleDic.end())
	{
		mPluginHandleDic.insert(PluginInstanceMap::value_type(strPluginName, plugin));
		plugin->Install();
	}
	else
	{
		std::cout << strPluginName << std::endl;
		assert(0);
	}
}

void EM::PluginManager::UnRegistered(IPlugin* plugin)
{
	PluginInstanceMap::iterator it = mPluginHandleDic.find(plugin->GetPluginName());
	if (it != mPluginHandleDic.end())
	{
		it->second->Uninstall();
		delete it->second;
		it->second = NULL;
		mPluginHandleDic.erase(it);
	}
}

bool EM::PluginManager::ReLoadPlugin(const std::string & strPluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who has been reloaded
	PluginInstanceMap::iterator itInstance = mPluginHandleDic.find(strPluginDLLName);
	if (itInstance == mPluginHandleDic.end())
	{
		return false;
	}
	//1
	IPlugin* pPlugin = itInstance->second;
	IModule* pModule = pPlugin->First();
	while (pModule)
	{
		pModule->BeforeShut();
		pModule->Shut();
		pModule->Finalize();

		pModule = pPlugin->Next();
	}

	//2
	PluginLibMap::iterator it = mPluginLibDic.find(strPluginDLLName);
	if (it != mPluginLibDic.end())
	{
		EMDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		mPluginLibDic.erase(it);
	}

	//3
	EMDynLib* pLib = new EMDynLib(strPluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		mPluginLibDic.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

		DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
		if (!pFunc)
		{
			std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
			assert(0);
			return false;
		}

		pFunc(this);
	}
	else
	{
#if EM_PLATFORM == EM_PLATFORM_LINUX
		char* error = dlerror();
		if (error)
		{
			std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
			std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
		}
#elif EM_PLATFORM == EM_PLATFORM_WIN
		std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
		std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
		assert(0);
		return false;
#endif // NF_PLATFORM
	}

	//4
	PluginInstanceMap::iterator itReloadInstance = mPluginHandleDic.begin();
	for (; itReloadInstance != mPluginHandleDic.end(); itReloadInstance++)
	{
		if (strPluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

bool EM::PluginManager::LoadPluginLibrary(const std::string& strPluginDLLName)
{
	PluginLibMap::iterator it = mPluginLibDic.find(strPluginDLLName);
	if (it == mPluginLibDic.end())
	{
		EMDynLib* pLib = new EMDynLib(strPluginDLLName);
		bool bLoad = pLib->Load();

		if (bLoad)
		{
			mPluginLibDic.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

			DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
			if (!pFunc)
			{
				std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
				assert(0);
				return false;
			}

			pFunc(this);

			return true;
		}
		else
		{
#if EM_PLATFORM == EM_PLATFORM_LINUX || EM_PLATFORM == EM_PLATFORM_APPLE
			char* error = dlerror();
			if (error)
			{
				std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
				std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
				assert(0);
				return false;
			}
#elif EM_PLATFORM == EM_PLATFORM_WIN
			std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
			std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
#endif // EM_PLATFORM
		}
	}

	return false;
}

bool EM::PluginManager::UnLoadPluginLibrary(const std::string& strPluginDLLName)
{
	PluginLibMap::iterator it = mPluginLibDic.find(strPluginDLLName);
	if (it != mPluginLibDic.end())
	{
		EMDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = 0;
		mPluginLibDic.erase(it);

		return true;
	}

	return false;
}