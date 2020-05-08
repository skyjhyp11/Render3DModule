#include "ObjHolder.h"



ObjHolder::ObjHolder()
{
}


ObjHolder::~ObjHolder()
{
}

int ObjHolder::SetObjs(const std::string &path)
{
	Help::Getfiles(m_objs, path, true);
	return m_objs.size();
}

std::vector<std::string> ObjHolder::GetObjs()
{
	return m_objs;
}
