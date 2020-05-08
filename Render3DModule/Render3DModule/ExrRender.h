#pragma once
#include "Help.h"
#include "Config.h"

class ExrRender
{
public:
	ExrRender();
	virtual ~ExrRender();

	void SetXmlsPath(const std::string &xmlsPath);
	void SetobjExrsPath(const std::string &objExrsPath);

	void SetObjID(const std::string &objID);
	void SetObjXmlPathIDs(const std::vector<std::string> &objXmlPathIDs);
	size_t CountTexture();
	void SetOldTexture(const std::string &textureOldPathID);
	void UpdateTexture(size_t index);
	void Start(size_t textureIndex);

private:
	void SetObjPath(const std::string &objPath);
	void MoveExr(size_t textureIndex);
	void SetObjXmlIDs(const std::vector<std::string> &objXmlPathIDs);

private:
	std::string m_objID;
	std::string m_xmlsPath;			// "F:\AlignTexture\RenderData\dst_xml\"
	std::string m_objTexturesPath;	// "F:\AlignTexture\RenderData\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_texture\"
	std::string m_objXmlsPath;		// "F:\AlignTexture\RenderData\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_xml\"
	std::string m_objExrsPath;		// "F:\AlignTexture\RenderData\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_exr\"

	std::string m_objTextureOldPathID; //F:\AlignTexture\RenderData\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\14042_750_mL_Wine_Bottle_dfinal.jpg
	std::vector<std::string> m_objTextureIDs;   // "{1.jpg, 2.png};"
	std::vector<std::string> m_objXmlPathIDs;	// "...\obj_1.xml, ...\obj_2.xml"
	std::vector<std::string> m_objXmlIDs;		// "obj_1, obj_2"

};
