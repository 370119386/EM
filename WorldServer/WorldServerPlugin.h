#ifndef __WORLD__SERVER__PLUGIN__H__
#define __WORLD__SERVER__PLUGIN__H__
#include "../EM/Common/Platform.h"
#include "../EM/SDK/IPlugin.h"
#include "../EM/PluginManager.h"

namespace EM
{
	class WorldServerPlugin : public IPlugin
	{
	public:
		WorldServerPlugin(PluginManager* p)
		{
			m_pPluginManager = p;
		}

		int GetPluginVersion() const;
		std::string GetPluginName() const;
		virtual void Install() override;
		virtual void Uninstall() override;
	};
}
#endif//__WORLD__SERVER__PLUGIN__H__