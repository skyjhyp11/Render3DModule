#pragma once
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Types.h"
extern IniAttributes iniAttributes;

class Statistics
{
public:
	Statistics();
	virtual ~Statistics();
	void AddObjStatistic(ObjStatistic objStatistic);
	const std::vector<ObjStatistic> & GetObjStatistic();
	void SetStatisticslogFile(const std::string & logName);
	void PrintFileStructure(const std::string & filePath, const std::string & comment = "");
	void PrintSummarize(StatisticsSummarizeParam statistics);

private:
	std::vector<ObjStatistic> m_objs;
	std::string m_logName;
};

