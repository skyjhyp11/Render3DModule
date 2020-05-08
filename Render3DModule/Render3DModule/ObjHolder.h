#pragma once
#include <vector>
#include <io.h>
#include <fstream>
#include <string>

#include "Help.h"

class ObjHolder
{
public:
	ObjHolder();
	virtual ~ObjHolder();
	
	//get objects in specified path for m_objs, return the number of objects
	int SetObjs(const std::string &objPath);
	std::vector<std::string> GetObjs();
private:
	std::vector<std::string> m_objs;
};

