#include "TargetRender.h"

#define  EXR_BATCH_COUNT 10
#define  DEBUG_DELETE_EXRFILE TRUE

TargetRender::TargetRender()
{
}


TargetRender::~TargetRender()
{
}

void TargetRender::SetObjID(const std::string & objID)
{
	m_objID = objID;
}

void TargetRender::SetobjExrsPath(const std::string & objExrsPath)
{
	m_objExrsPath = objExrsPath;
}

void TargetRender::SetTargetsPath(const std::string & objTargetsPath)
{
	m_objTargetsPath = objTargetsPath;
}

void TargetRender::Start()
{
	//1. 获取 m_objExrsPath 所有exr文件
	Help::GetSpecifiedTypefiles(m_objExrPathIDs, m_objExrsPath, "exr", true);

	//2. 执行渲染生成png, 5个渲染一次
	// processing batch, XML_BATCH_COUNT files at a time.
	std::string curentBatchExrNames;
	size_t currentExrCount;
	//std::string cmd;
	for (size_t index = 0; index < m_objExrPathIDs.size(); ++index)
	{
		if (0 == index % iniAttributes.exrBatchCount)//EXR_BATCH_COUNT
		{
			curentBatchExrNames.clear();
			currentExrCount = 0;
		}
		curentBatchExrNames.append(" ").append(m_objExrPathIDs[index]);
		++currentExrCount;

		// generate exr files
		if (currentExrCount == iniAttributes.exrBatchCount || index == m_objExrPathIDs.size() - 1) //EXR_BATCH_COUNT
		{
			cmd = createPngByMitsuba + curentBatchExrNames;
			std::cout << "INFO: " << m_objID << ": generating " << index << "th png file..." << std::endl;
			std::cout << "INFO: " << "command > \"" << cmd << "\"" << std::endl;

			if (iniAttributes.debugLogDetails)
				my_logger->info("    |  command > \"{}\"", cmd);

			system(cmd.c_str());
		}
	}

	//move targets to dst_png folder
	MoveTarget();
	my_logger->info("  [{}]: generated targets to \"{}\" successfully.", m_objID, m_objTargetsPath);

	// delete exr file for saving disk space.
	if (iniAttributes.debugDeleteExrFile) // DEBUG_DELETE_EXRFILE
	{
		Help::ClearFolder(m_objExrsPath);
	}

	my_logger->flush();
}

void TargetRender::MoveTarget()
{
	std::string targetInExrFileName;
	std::string targetOutPngFileName;
	std::vector<std::string> objExrs(m_objExrPathIDs);
	size_t posPathPrefix;
	size_t objPngCounter = 0;
	for (size_t i = 0; i < objExrs.size(); ++i)
	{
		objExrs[i].erase(objExrs[i].end() - 4, objExrs[i].end());
		targetInExrFileName = objExrs[i] + ".png";
		posPathPrefix = objExrs[i].find_last_of("\\");
		targetOutPngFileName = m_objTargetsPath + objExrs[i].substr(posPathPrefix + 1) + ".png";

		if (Help::FileCopy(targetInExrFileName, targetOutPngFileName, true))
		{
			++objPngCounter;
		}

		remove(targetInExrFileName.c_str());
	}

	currentObjStatistic.actualSum = objPngCounter;

}
