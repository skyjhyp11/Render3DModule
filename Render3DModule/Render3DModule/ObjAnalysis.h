#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <io.h>
#include <fstream>
#include <assert.h>
#include <algorithm> 

#include "Help.h"

#define OBJ_BASE_SCALE	(3.0)

class ObjAnalysis
{
public:
	ObjAnalysis();
	virtual ~ObjAnalysis();
	void SetObj(const std::string &objsPath, const std::string &objID);
	std::string GetObjPathID();
	std::string GetObjPath();

	std::string GetTexturePathID();
	void Calculate();
	float GetShiftX();
	float GetShiftY();
	float GetShiftZ();
	float GetScale();

private:
	std::string GotFileTypeByPostfix(std::string filename);

private:
	std::string m_objPathID;	// "F:\AlignTexture\RenderData\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\14042_750_mL_Wine_Bottle_r_v1_L3_john.obj"
	std::string m_objPath;		// "F:\AlignTexture\RenderData\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\"
	std::string m_texturePathID;// "F:\AlignTexture\RenderData\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\14042_750_mL_Wine_Bottle_dfinal.jpg"

	std::string m_texture;	// "14042_750_mL_Wine_Bottle_dfinal.jpg"
	std::string m_mtl;		// "14042_750_mL_Wine_Bottle_r_v1_L3.mtl"

	float m_maxX;
	float m_maxY;
	float m_maxZ;
	float m_minX;
	float m_minY;
	float m_minZ;

	float m_shiftX;
	float m_shiftY;
	float m_shiftZ;

	float m_scale;
};

