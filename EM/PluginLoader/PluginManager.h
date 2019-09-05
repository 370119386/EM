#ifndef __PLUGIN__MANAGER__H__
#define __PLUGIN__MANAGER__H__
#include "Common/Platform.h"
#include "Common/Singleton.h"
#include <map>
#include <string>

namespace EM
{
	class IPlugin;
	class EMDynLib;
	class PluginManager : public Singleton<PluginManager>
	{
	public:
		PluginManager();
		~PluginManager();

		void LoadPlugin();
		bool Awake();
		bool Init();
		bool AfterInit();
		bool CheckConfig();
		bool ReadyExecute();
		bool Execute();
		bool BeforeShut();
		bool Shut();
		bool Finalize();

	public:
		void Registered(IPlugin* plugin);
		void UnRegistered(IPlugin* plugin);
		bool ReLoadPlugin(const std::string & strPluginDLLName);

	protected:
		bool LoadPluginLibrary(const std::string& strPluginDLLName);
		bool UnLoadPluginLibrary(const std::string& strPluginDLLName);

	private:
		typedef std::map<std::string, bool> PluginNameMap;
		typedef std::map<std::string, EMDynLib*> PluginLibMap;
		typedef std::map<std::string, IPlugin*> PluginInstanceMap;

		typedef void(*DLL_START_PLUGIN_FUNC)(PluginManager* pm);
		typedef void(*DLL_STOP_PLUGIN_FUNC)(PluginManager* pm);

	private:
		PluginNameMap mPluginNameDic;
		PluginLibMap mPluginLibDic;
		PluginInstanceMap mPluginHandleDic;

	};
}
#endif