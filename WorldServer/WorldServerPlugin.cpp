#include "WorldServerPlugin.h"
using namespace EM;

#ifdef EM_DYNAMIC_PLUGIN
EM_EXPORT void DllStartPlugin(PluginManager* pm)
{
	//CREATE_PLUGIN(pm, WorldServerPlugin);
};

EM_EXPORT void DllStopPlugin(PluginManager* pm)
{
	//DESTROY_PLUGIN(pm, WorldServerPlugin);
};
#endif

int EM::WorldServerPlugin::GetPluginVersion() const
{
	return 0;
}

std::string EM::WorldServerPlugin::GetPluginName() const
{
	return GET_CLASS_NAME(WorldServerPlugin);
}

void WorldServerPlugin::Install()
{

}

void WorldServerPlugin::Uninstall()
{

}