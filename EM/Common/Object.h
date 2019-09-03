#ifndef __EM__OBJECT__H__
#define __EM__OBJECT__H__

#include <new>
#include "MemObjectTracer.h"
#include "Platform.h"

namespace EM
{
	class Object
	{
	protected:
		Object() {}

	public:
#if EM_MEM_LEACK_CHECK
		void *operator new(size_t size, const char* fileName, const char* fcuntionName, int line)
		{
			void* p = ::malloc(size);
			MemObjectTracer::Instance()->AddMemInfo(p, fileName, fcuntionName, line, size);
			return p;
		}

		void operator delete(void * p)
		{
			MemObjectTracer::Instance()->DelMemInfo(p);
			::free(p);
		}
#endif
	public:
		virtual ~Object() {}
	};
}

#if EM_MEM_LEACK_CHECK
#define EMNew new(__FILE__,__FUNCDNAME__,__LINE__)
#define  EMDelete delete
#else
#define EMNew new
#define  EMDelete delete
#endif
#endif//__EM__OBJECT__H__