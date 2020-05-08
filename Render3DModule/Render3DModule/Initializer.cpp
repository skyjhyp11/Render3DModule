#include "Initializer.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

Initializer::Initializer()
{
}


Initializer::~Initializer()
{
}

bool Initializer::ReadIniFile()
{
	if (ReadIniFileAttributes() && ReadIniFileObjsBaseInfo())
	{
		return true;
	}
	return false;
}

bool Initializer::ReadIniFileAttributes()
{
	m_iniAttributesFile = Help::GetExePathByWinAPI() + "\\" + ini_file_config;
	std::cout << "INFO: int File path: " << m_iniAttributesFile << std::endl;

	//char path[1024] = {0};
	//_getcwd(path, 1024);
	//std::cout << "INFO: int File path: " << path << std::endl;
	//返回值:调试时: 本文件所在目录, 执行exe时,才返回exe路径
	//assert(false);

	// if this file not exist, create a new one and set default values.
	if (0 != _access(m_iniAttributesFile.c_str(), 0))
	{
		std::ofstream out;
		out.open(m_iniAttributesFile, std::ios::out);
		if (out.fail())
		{
			std::cout << "ERROR: Fail to create int file." << std::endl;
			my_logger->error("       create int file failed in function {} line {} !!!", __FUNCTION__, __LINE__);
			out.close();
			return FALSE;
		}
		else
		{
			std::cout << "INFO: create a ini File using default setting." << std::endl;
			out << GetIniFileDefaultSetting();
			out.close();
			m_iniAttributes = IniAttributes();
			return TRUE;
		}
	}
	//file exist
	else
	{
		// get every attributes
		std::cout << "INFO: begin to read initialized setting from " << m_iniAttributesFile << std::endl;

#ifdef __WINDOWS__
		//SetIniAttributesByWinAPI();

		//test boost
		SetIniAttributesByBoost();
#else
		SetIniAttributesByBoost();
#endif // __WINDOWS__

		return TRUE;
	}
	return FALSE;
}

bool Initializer::ReadIniFileObjsBaseInfo()
{
	m_iniObjBaseInfoFile = Help::GetExePathByWinAPI() + "\\" + ini_file_object_basic_info;
	std::cout << "INFO: int ObjBaseInfo File path: " << m_iniObjBaseInfoFile << std::endl;

	// if this file not exist, assert.
	if (0 != _access(m_iniObjBaseInfoFile.c_str(), 0))
	{
		std::cout << "ERR: open ini file ObjBaseInfo failed: " << m_iniObjBaseInfoFile << std::endl;
		assert(false);
	}
	//file exist
	else
	{
		// get every attributes
		std::cout << "INFO: begin to read initialized ObjBaseInfo setting from " << m_iniObjBaseInfoFile << std::endl;

#ifdef __WINDOWS__
		//SetIniAttributesByWinAPI();

		//test boost
		SetIniObjBaseInfoByBoost();
#else
		SetIniAttributesByBoost();
#endif // __WINDOWS__

		return TRUE;
	}
	return false;
}

IniAttributes Initializer::GetIniAttributes()
{
	return m_iniAttributes;
}

std::set<ObjBaseInfo> &Initializer::GetIniObjsBaseInfo()
{
	return m_objsBaseInfo;
}

std::string Initializer::GetIniFileDefaultSetting()
{
	std::string defaultSetting(
		std::string("[Global]\n") +
		std::string("DEBUG_LOG_DETAILS = true\n\n") +
		std::string("; If the cpu status is ok, program don't need sleep\n") +
		std::string("PERFORMANCE_NEED_SLEEP = true\n") +
		std::string("PERFORMANCE_SLEEP_SECOND = 15\n\n") +
		std::string("[ObjAnalysis]\n") +
		std::string("OBJ_BASE_SCALE = 3.0\n\n") +
		std::string("[ExrRender]\n") +
		std::string("; the number once time program can create xml file from exr file\n") +
		std::string("XML_BATCH_COUNT = 4\n\n") +
		std::string("[TargetRender]\n") +
		std::string("; the number once time program can create exr file from png file\n") +
		std::string("EXR_BATCH_COUNT = 16\n") +
		std::string("DEBUG_DELETE_EXRFILE = false\n\n") +
		std::string("[XmlCreator]\n") +
		std::string("DEBUG_INCREASE_XML = false\n") +
		std::string("STEP_SCALE = 0.05\n") +
		std::string("STEP_ANGLE = 45\n") +
		std::string("STEP_EMITTER = 0.5\n\n") +
		std::string("[Statistics]\n") +
		std::string("STATISTICS_MAX_OBJID_SIZE = 32\n")
	);

	return defaultSetting;
}

bool Initializer::SetIniAttributesByWinAPI()
{
	int tmpIntValue = 0;
	std::string tmpString;
	wchar_t tmpWChar[256];
	memset(tmpWChar, 0x00, sizeof(tmpWChar));

	//std::wstring wIniFile = Help::ConvertStdStringToStdWString(m_iniAttributesFile);
	//const wchar_t *iniFile = wIniFile.c_str();

	const wchar_t *iniFile = Help::ConvertStdStringToWChar(m_iniAttributesFile);

	// [Global]
	::GetPrivateProfileString(L"Global", L"CREATE_MASK_MODULE", L"false", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.createMaskModule = (tmpString == "true") ? true : false;

	::GetPrivateProfileString(L"Global", L"DEBUG_LOG_DETAILS", L"true", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugLogDetails = (tmpString == "true") ? true : false;

	::GetPrivateProfileString(L"Global", L"PERFORMANCE_NEED_SLEEP", L"true", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.performanceNeedSleep = (tmpString == "true") ? true : false;

	m_iniAttributes.performanceSleepSecond = ::GetPrivateProfileInt(L"Global", L"PERFORMANCE_SLEEP_SECOND", 15, iniFile);

	// [ObjAnalysis]
	::GetPrivateProfileString(L"ObjAnalysis", L"OBJ_BASE_SCALE", L"3.0", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	m_iniAttributes.objBaseScale = std::stof(tmpString);

	// [ExrRender]
	m_iniAttributes.xmlBatchCount = ::GetPrivateProfileInt(L"ExrRender", L"XML_BATCH_COUNT", 3, iniFile);

	// [TargetRender]
	m_iniAttributes.exrBatchCount = ::GetPrivateProfileInt(L"TargetRender", L"EXR_BATCH_COUNT", 10, iniFile);

	::GetPrivateProfileString(L"TargetRender", L"DEBUG_DELETE_EXRFILE", L"false", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugDeleteExrFile = (tmpString == "true") ? true : false;

	// [XmlCreator]
	::GetPrivateProfileString(L"XmlCreator", L"DEBUG_INCREASE_XML", L"false", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugIncreaseXml = (tmpString == "true") ? true : false;

	::GetPrivateProfileString(L"XmlCreator", L"STEP_SCALE", L"0.05", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	m_iniAttributes.stepScale = std::stof(tmpString);

	m_iniAttributes.stepAngle = ::GetPrivateProfileInt(L"XmlCreator", L"STEP_ANGLE", 45, iniFile);

	::GetPrivateProfileString(L"XmlCreator", L"STEP_EMITTER", L"0.5", tmpWChar, sizeof(tmpWChar), iniFile);
	tmpString = Help::ConvertWCharToStdString(tmpWChar);
	m_iniAttributes.stepEmitter = std::stof(tmpString);

	// [Statistics]
	m_iniAttributes.statisticsMaxObjIDSize = ::GetPrivateProfileInt(L"Statistics", L"STATISTICS_MAX_OBJID_SIZE", 32, iniFile);

	//std::cout << m_iniAttributes.debugLogDetails  << std::endl
	//	<< m_iniAttributes.performanceNeedSleep	  << std::endl
	//	<< m_iniAttributes.performanceSleepSecond << std::endl
	//	<< m_iniAttributes.objBaseScale			  << std::endl
	//	<< m_iniAttributes.xmlBatchCount		  << std::endl
	//	<< m_iniAttributes.exrBatchCount		  << std::endl
	//  << m_iniAttributes.exrBatchCount          << std::endl
	//	<< m_iniAttributes.debugIncreaseXml		  << std::endl
	//	<< m_iniAttributes.stepScale			  << std::endl
	//	<< m_iniAttributes.stepAngle			  << std::endl
	//	<< m_iniAttributes.stepEmitter			  << std::endl
	//	<< m_iniAttributes.statisticsMaxObjIDSize << std::endl;

	return true;
}

bool Initializer::SetIniAttributesByBoost()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(m_iniAttributesFile, pt);
	std::string tmpString;
	//size_t tmpInt;

	// [Global]
	tmpString = pt.get<std::string>("Global.CREATE_MASK_MODULE");
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.createMaskModule = (tmpString == "true") ? true : false;

	tmpString = pt.get<std::string>("Global.DEBUG_LOG_DETAILS");
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugLogDetails = (tmpString == "true") ? true : false;

	tmpString = pt.get<std::string>("Global.PERFORMANCE_NEED_SLEEP");
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.performanceNeedSleep = (tmpString == "true") ? true : false;

	m_iniAttributes.performanceSleepSecond = pt.get<size_t>("Global.PERFORMANCE_SLEEP_SECOND");

	// [ObjAnalysis]
	m_iniAttributes.objBaseScale = pt.get<float>("ObjAnalysis.OBJ_BASE_SCALE");

	// [ExrRender]
	m_iniAttributes.xmlBatchCount = pt.get<size_t>("ExrRender.XML_BATCH_COUNT");

	// [TargetRender]
	m_iniAttributes.exrBatchCount = pt.get<size_t>("TargetRender.EXR_BATCH_COUNT");

	tmpString = pt.get<std::string>("TargetRender.DEBUG_DELETE_EXRFILE");
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugDeleteExrFile = (tmpString == "true") ? true : false;

	// [XmlCreator]
	tmpString = pt.get<std::string>("XmlCreator.DEBUG_INCREASE_XML");
	transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
	m_iniAttributes.debugIncreaseXml = (tmpString == "true") ? true : false;

	m_iniAttributes.stepScale = pt.get<float>("XmlCreator.STEP_SCALE");
	m_iniAttributes.stepAngle = pt.get<size_t>("XmlCreator.STEP_ANGLE");
	m_iniAttributes.stepEmitter = pt.get<float>("XmlCreator.STEP_EMITTER");

	// [Statistics]
	m_iniAttributes.statisticsMaxObjIDSize = pt.get<size_t>("Statistics.STATISTICS_MAX_OBJID_SIZE");

	return true;
}

//每添加一个obj就需要在此读取其初始属性
bool Initializer::SetIniObjBaseInfoByBoost()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(m_iniObjBaseInfoFile, pt);
	ObjBaseInfo tmpObjBaseInfo;

	// [14042_750_mL_Wine_Bottle_r_v1_L3]
	tmpObjBaseInfo.objID = "14042_750_mL_Wine_Bottle_r_v1_L3";
	tmpObjBaseInfo.Roi.leftTopX = pt.get<size_t>("14042_750_mL_Wine_Bottle_r_v1_L3.LeftTop_X");
	tmpObjBaseInfo.Roi.leftTopY = pt.get<size_t>("14042_750_mL_Wine_Bottle_r_v1_L3.LeftTop_Y");
	tmpObjBaseInfo.Roi.rightBottomX = pt.get<size_t>("14042_750_mL_Wine_Bottle_r_v1_L3.RightBottom_X");
	tmpObjBaseInfo.Roi.rightBottomY = pt.get<size_t>("14042_750_mL_Wine_Bottle_r_v1_L3.RightBottom_Y");

	tmpObjBaseInfo.baseInfo.objBaseScale = pt.get<float>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjBaseScale");
	tmpObjBaseInfo.baseInfo.objBaseRotateX = pt.get<int>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjBaseRotate_X");
	tmpObjBaseInfo.baseInfo.objBaseRotateY = pt.get<int>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjBaseRotate_Y");
	tmpObjBaseInfo.baseInfo.objBaseRotateZ = pt.get<int>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjBaseRotate_Z");

	tmpObjBaseInfo.baseInfo.objBaseTranslateX = pt.get<float>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjTranslate_X");
	tmpObjBaseInfo.baseInfo.objBaseTranslateY = pt.get<float>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjTranslate_Y");
	tmpObjBaseInfo.baseInfo.objBaseTranslateZ = pt.get<float>("14042_750_mL_Wine_Bottle_r_v1_L3.ObjTranslate_Z");

	m_objsBaseInfo.insert(tmpObjBaseInfo);

	// [coca-cola_jo]
	tmpObjBaseInfo.objID = "coca-cola_jo";
	tmpObjBaseInfo.Roi.leftTopX = pt.get<size_t>("coca-cola_jo.LeftTop_X");
	tmpObjBaseInfo.Roi.leftTopY = pt.get<size_t>("coca-cola_jo.LeftTop_Y");
	tmpObjBaseInfo.Roi.rightBottomX = pt.get<size_t>("coca-cola_jo.RightBottom_X");
	tmpObjBaseInfo.Roi.rightBottomY = pt.get<size_t>("coca-cola_jo.RightBottom_Y");

	tmpObjBaseInfo.baseInfo.objBaseScale = pt.get<float>("coca-cola_jo.ObjBaseScale");
	tmpObjBaseInfo.baseInfo.objBaseRotateX = pt.get<int>("coca-cola_jo.ObjBaseRotate_X");
	tmpObjBaseInfo.baseInfo.objBaseRotateY = pt.get<int>("coca-cola_jo.ObjBaseRotate_Y");
	tmpObjBaseInfo.baseInfo.objBaseRotateZ = pt.get<int>("coca-cola_jo.ObjBaseRotate_Z");

	tmpObjBaseInfo.baseInfo.objBaseTranslateX = pt.get<float>("coca-cola_jo.ObjTranslate_X");
	tmpObjBaseInfo.baseInfo.objBaseTranslateY = pt.get<float>("coca-cola_jo.ObjTranslate_Y");
	tmpObjBaseInfo.baseInfo.objBaseTranslateZ = pt.get<float>("coca-cola_jo.ObjTranslate_Z");

	m_objsBaseInfo.insert(tmpObjBaseInfo);

	return false;
}
