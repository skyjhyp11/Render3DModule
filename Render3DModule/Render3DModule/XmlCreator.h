#pragma once
#include "stdlib.h"

#include "tinyXml.h"
#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "tinyxmlSTL.lib")

#include "Help.h"
#include "Config.h"

//extern std::shared_ptr<spdlog::logger> my_logger;

class XmlCreator
{
public:
	XmlCreator();
	virtual ~XmlCreator();

	void SetXmlsPath(const std::string &xmlsPath);
	void SetObjID(const std::string &objID);
	void SetObjPathID(const std::string &objPathID);
	void SetObjParameter(const ParameterXml &paraXml);
	std::string GetObjXmlPath() const;
	size_t Start();

private:
	void ModifySeceneXmlFile(const std::string &xmlFileName, const ParameterXmlModify &para);

private:
	std::string m_xmlsPath;		//such as "F:\AlignTexture\RenderData\dst_xml\"
	std::string m_objID;		//such as "14042_750_mL_Wine_Bottle_r_v1_L3"
	std::string m_objXmlPath;	// such as "F:\AlignTexture\RenderData\dst_xml\14042_750_mL_Wine_Bottle_r_v1_L3_xml\"
	std::string m_objPathID;
	ParameterXml m_paraXml;
};

