#include "ExrRender.h"

#define  XML_BATCH_COUNT 3

ExrRender::ExrRender()
{
}


ExrRender::~ExrRender()
{
}

void ExrRender::SetXmlsPath(const std::string & xmlsPath)
{
	//such as "F:\AlignTexture\RenderData\dst_xml\"
	m_xmlsPath = xmlsPath;
}

void ExrRender::SetobjExrsPath(const std::string & objExrsPath)
{
	m_objExrsPath = objExrsPath;
}

void ExrRender::SetObjID(const std::string & objID)
{
	m_objID = objID;
}

void ExrRender::SetObjXmlPathIDs(const std::vector<std::string> &objXmlPathIDs)
{
	m_objXmlPathIDs = objXmlPathIDs;
	SetObjXmlIDs(objXmlPathIDs);
}

void ExrRender::SetObjPath(const std::string & objPath)
{
}


size_t ExrRender::CountTexture()
{
	m_objTexturesPath = m_xmlsPath + m_objID + "_texture\\";
	m_objXmlsPath = m_xmlsPath + m_objID + "_xml\\";

	Help::Getfiles(m_objTextureIDs, m_objTexturesPath);
	//std::cout
	//Help::printVector(m_objTextureIDs);
	return m_objTextureIDs.size();
}

void ExrRender::SetOldTexture(const std::string & textureOldPathID)
{
	m_objTextureOldPathID = textureOldPathID;
}

void ExrRender::UpdateTexture(size_t index)
{
	Help::FileCopy(std::string(m_objTexturesPath + m_objTextureIDs[index]), m_objTextureOldPathID, true);
	my_logger->info("    |* update texture: \"{}\" -> \"{}\"", m_objTextureIDs[index], m_objTextureOldPathID);
}

void ExrRender::Start(size_t textureIndex)
{
	// processing batch, XML_BATCH_COUNT files at a time.
	std::string curentBatchXmlNames;
	size_t currentXmlCount;
	//std::string cmd;
	size_t index;
	for (index = 0; index < m_objXmlPathIDs.size(); ++index)
	{
		if (0 == index % iniAttributes.xmlBatchCount) // XML_BATCH_COUNT
		{
			curentBatchXmlNames.clear();
			currentXmlCount = 0;
		}
		curentBatchXmlNames.append(" ").append(m_objXmlPathIDs[index]);
		++currentXmlCount;

		// generate exr 
		if (currentXmlCount == iniAttributes.xmlBatchCount || index == m_objXmlPathIDs.size() - 1) //XML_BATCH_COUNT
		{
			cmd = createExrByMitsuba + curentBatchXmlNames;
			std::cout << m_objID << ": rendering " << index << "th exr file..." << std::endl;
			std::cout << "command > \"" << cmd << "\"" << std::endl;

			if (iniAttributes.debugLogDetails)  //DEBUG_LOG_DETAILS
				my_logger->info("    |  command > \"{}\"", cmd);

			system(cmd.c_str());  // for debug, should release

			if (iniAttributes.performanceNeedSleep)//PERFORMANCE_NEED_SLEEP
			{
				Sleep(iniAttributes.performanceSleepSecond * 1000);
				std::cout << "sleep " << iniAttributes.performanceSleepSecond << "s......." << std::endl;
				my_logger->info("    |  sleep {}s...", iniAttributes.performanceSleepSecond);
			}
			my_logger->flush();
		}
	}

	// moveExr  to "objID_exr" folder and rename
	// rename reason: different textures use the same xmls.
	if (index == m_objXmlPathIDs.size()) //未生成就移动!!!!
	{
		MoveExr(textureIndex);
	}
}

void ExrRender::MoveExr(size_t textureIndex)
{
	std::string exrInXmlFileName;
	std::string exrOutExrFileName;
	std::vector<std::string> objXmls(m_objXmlPathIDs);

	for (size_t i = 0; i < objXmls.size(); ++i)
	{
		objXmls[i].erase(objXmls[i].end() - 4, objXmls[i].end());
		exrInXmlFileName = objXmls[i] + ".exr";
		exrOutExrFileName = m_objExrsPath + m_objXmlIDs[i] + "_" + std::to_string(textureIndex + 1) + ".exr";

		Help::FileCopy(exrInXmlFileName, exrOutExrFileName, true);

		remove(exrInXmlFileName.c_str()); // for debug, should release

	}
}

//such as objXmlPathIDs[0] = "F:\\AlignTexture\\RenderData\\dst_xml\\14042_750_mL_Wine_Bottle_r_v1_L3_xml\\14042_750_mL_Wine_Bottle_r_v1_L3_1.xml"
// m_objXmlIDs = "14042_750_mL_Wine_Bottle_r_v1_L3_1"
void ExrRender::SetObjXmlIDs(const std::vector<std::string>& objXmlPathIDs)
{
	m_objXmlIDs.clear();
	std::string xmlName;
	size_t posPathPrefix;

	for each (std::string var in objXmlPathIDs)
	{
		posPathPrefix = var.find_last_of("\\");
		xmlName = var.substr(posPathPrefix + 1);
		xmlName.erase(xmlName.end() - 4, xmlName.end());
		m_objXmlIDs.insert(m_objXmlIDs.end(), xmlName);
	}
}
