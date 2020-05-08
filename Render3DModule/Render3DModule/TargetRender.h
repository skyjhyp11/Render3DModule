#pragma once
#include "Help.h"
#include "Config.h"

class TargetRender
{
public:
	TargetRender();
	virtual ~TargetRender();
	
	void SetObjID(const std::string & objID);
	void SetobjExrsPath(const std::string &objExrsPath);
	void SetTargetsPath(const std::string &objTargetsPath);
	void Start();

private:
	void MoveTarget();
private:
	std::string m_objID;
	std::string m_objExrsPath;		// "F:\AlignTexture\RenderData\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_exr\"
	std::string m_objTargetsPath;   // "F:\AlignTexture\RenderData\dst_png\"
	std::vector<std::string> m_objExrPathIDs;	// "...\obj_1.exr, ...\obj_2.exr"

};

