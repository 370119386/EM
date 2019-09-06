#ifndef __MEMORY__COUNTER__H__
#define __MEMORY__COUNTER__H__

#include <string>
#include <map>
#include "../Common/Platform.h"

namespace EM
{
    template <class T>
    class MemoryCounter
    {
    private:
        MemoryCounter() {}
    public:
        MemoryCounter(const std::string& strClassName)
        {
            mstrClassName = strClassName;
            mxCounter[mstrClassName]++;
        }

        virtual ~MemoryCounter()
        {
            mxCounter[mstrClassName]--;
        }

        std::string mstrClassName;
        static std::map<std::string, int> mxCounter;
    };

    template <class T>
    std::map<std::string, int> MemoryCounter<T>::mxCounter;
}

#endif//__MEMORY__COUNTER__H__