#include "EMDynLib.h"
#include <string>

bool EM::EMDynLib::Load()
{
    std::string strLibPath = "./";
    strLibPath += mstrName;
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strLibPath.c_str());

    return mInst != 0;
}

bool EM::EMDynLib::UnLoad()
{
    DYNLIB_UNLOAD(mInst);
    return true;
}

void* EM::EMDynLib::GetSymbol(const char* szProcName)
{
    return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}