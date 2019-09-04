#ifndef __MEMOBJECT__TRACER__H__
#define __MEMOBJECT__TRACER__H__
#include "Singleton.h"
#include <map>

namespace EM
{
	class MemObjectTracer : public Singleton<MemObjectTracer>
	{
	public:
		MemObjectTracer();
		~MemObjectTracer();

		void AddMemInfo(void* p,const char* pcFile, const char* pcFunction, int line, size_t bytes);
		void DelMemInfo(void* p);

	private:
		struct MemInfo
		{
			const char* pcFile;
			const char* pcFunction;
			int line;
			size_t bytes;
		};
		std::map<void*, MemInfo*> m_memImpl;

	};
}
#endif//__MEMOBJECT__TRACER__H__