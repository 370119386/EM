#include "MemObjectTracer.h"
#include <assert.h>
#include <iostream>
#include <fstream>
using namespace EM;

MemObjectTracer::MemObjectTracer()
{
	m_memImpl.clear();
}

MemObjectTracer::~MemObjectTracer()
{
	std::map<void*, MemInfo*>::iterator end = m_memImpl.end();
	if (m_memImpl.size() > 0)
	{
		char buf[1024];
		std::ofstream outfile;
		outfile.open("MemLeak.txt", std::ios::out | std::ios::trunc);
		if (outfile)
		{
			for (std::map<void*, MemInfo*>::iterator itr = m_memImpl.begin(); itr != end; itr++)
			{
				sprintf(buf, "[memleak]:%d bytes [file]:%s [func]:%s line:%d \n",
					itr->second->bytes,
					itr->second->pcFile, itr->second->pcFunction, itr->second->line);
				outfile << buf;
			}
		}
		outfile.flush();
		outfile.close();
		for (std::map<void*, MemInfo*>::iterator itr = m_memImpl.begin(); itr != end; itr++)
		{
			delete itr->second;
		}
		m_memImpl.clear();
	}
}

void EM::MemObjectTracer::AddMemInfo(void* p, const char* pcFile, const char* pcFunction, int line, size_t bytes)
{
	MemInfo* pkMemInfo = new MemInfo();
	pkMemInfo->pcFunction = pcFunction;
	pkMemInfo->pcFile = pcFile;
	pkMemInfo->line = line;
	pkMemInfo->bytes = bytes;
	assert(m_memImpl.find(p) == m_memImpl.end());
	m_memImpl.insert(std::make_pair(p, pkMemInfo));
}

void EM::MemObjectTracer::DelMemInfo(void* p)
{
	assert(m_memImpl.find(p) != m_memImpl.end());
	delete m_memImpl[p];
	m_memImpl.erase(p);
}