#pragma once
//#include <string>
//#include <vector>
#include "Help.h"

class LabelChanger
{
public:
	LabelChanger();
	virtual ~LabelChanger();

	void Setlabels(const std::string & labelsPath);
	void SetSrcTexturePathID(const std::string & srcTexturePathID);
	void SetObj(const std::string & objID);
	void SetXmlsPath(const std::string & xmlsPath);
	void SetObjRoi(const Roi & objRoi);

	void Start();
private:
	std::vector<std::string> m_labels;	// "...\1.jpg; ...\2.png; ...\3.bmp"
	std::string m_objSrcTexturePathID;		//"...\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\14042_750_mL_Wine_Bottle_dfinal.jpg"
	std::string m_objID;
	std::string m_xmlsPath;
	std::string m_objDstTexturesPath;		//"...\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_texture\"
	Roi m_objRoi;
};
