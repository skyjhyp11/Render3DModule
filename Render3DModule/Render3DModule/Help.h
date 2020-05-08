#pragma once
#ifndef __HELP_H__
#define __HELP_H__

#define __WINDOWS__

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <io.h>
#include <direct.h>
#include <algorithm> 

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "Types.h"
#include "Statistics.h"

#define DEBUG_LOG_DETAILS         TRUE
#define PERFORMANCE_NEED_SLEEP    TRUE
#define PERFORMANCE_SLEEP_SECOND  10

extern std::shared_ptr<spdlog::logger> my_logger;
extern Statistics statistics;
extern ObjStatistic currentObjStatistic;
extern IniAttributes iniAttributes;

class Help
{
public:
	Help();
	~Help();
	template <class T>
	static void printVector(const std::vector<T> &v);
	static int StringSplit(std::vector<std::string> &dst, const std::string &src, const std::string &separator);
	static bool FileCopy(std::string &srcFile, std::string &dstFile, bool needOverwrite = false);
	static void Getfiles(std::vector<std::string> &outfileNames,const std::string &path, bool getDirectory = false, bool getFullName = false);
	static void GetSpecifiedTypefiles(std::vector<std::string> &outfileNames, const std::string &path, const std::string &fileType, bool getFullName = false);
	static std::string ReadFileIntoString(const std::string &filename);
	static int CreateFolder(const std::string &folderName);
	static void ClearFolder(const std::string &folderName);
	static std::string GetExePathByWinAPI();	// "D:\test_3D\Render3DModule\Debug"
	
	//convert a WChar string to a Ansi string; can support chinese charactor; use windows API.
	static std::string ConvertWCharToStdString(const wchar_t *wStr);
	static std::string ConvertStdWStringToStdString(std::wstring & inputws);

	//Converting a Ansi string to WChar string  
	static std::wstring ConvertStdStringToStdWString(std::string srcString);
	static const wchar_t * ConvertStdStringToWChar(std::string srcString);

};

template<class T>
inline void Help::printVector(const std::vector<T> &v)
{
	std::ostream_iterator<std::string> myout(std::cout, "\n");
	std::copy(v.begin(), v.end(), myout);
}
#endif /*__HELP_H__*/