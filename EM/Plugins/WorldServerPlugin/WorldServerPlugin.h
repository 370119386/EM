#ifndef __WORLD__SERVER__PLUGIN__H__
#define __WORLD__SERVER__PLUGIN__H__
#include "Common/Platform.h"
#include "Interface/IPlugin.h"
#include "PluginLoader/PluginManager.h"

namespace EM
{
	class WorldServerPlugin : public IPlugin
	{
	public:
		WorldServerPlugin(PluginManager* p)
		{
			m_pPluginManager = p;
		}

		int GetPluginVersion() const override;
		std::string GetPluginName() const override;
		virtual void Install() override;
		virtual void Uninstall() override;
	};
}
#endif//__WORLD__SERVER__PLUGIN__H__