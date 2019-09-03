#include <stdio.h>
#include "Common/Object.h"

int main()
{
	EM::MemObjectTracer::Instance()->Release();
    return 0;
}