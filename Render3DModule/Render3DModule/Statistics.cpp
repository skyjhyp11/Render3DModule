#include "Statistics.h"

#define STATISTICS_MAX_OBJID_SIZE 30


Statistics::Statistics()
{
}


Statistics::~Statistics()
{
}

void Statistics::AddObjStatistic(ObjStatistic objStatistic)
{
	m_objs.insert(m_objs.end(), objStatistic);
}

const std::vector<ObjStatistic> & Statistics::GetObjStatistic()
{
	return m_objs;
}

void Statistics::SetStatisticslogFile(const std::string & logName)
{
	m_logName = logName;
	try
	{
		std::fstream fout(logName, std::ios::out | std::ios::trunc);
		fout.close();
	}
	catch (std::exception& e)
	{
		std::cout<< "error happened:" << e.what() << std::endl;
	}
}

void Statistics::PrintFileStructure(const std::string & filePath, const std::string & comment)
{
	std::string separator = "=====================================";
	std::string comments = separator + " manifest (" + comment + ") " + separator;

	std::string cmd = "echo " + comments + " >> " + m_logName;
	system(cmd.c_str());

	cmd = "tree /f " + filePath + " >> " + m_logName;
	system(cmd.c_str());
}

void Statistics::PrintSummarize(StatisticsSummarizeParam statistics)
{
	std::ofstream out;
	// append  summarize to log file
	out.open(m_logName, std::ios::app);
	if (out.fail())
	{
		std::cout << "ERROR: Fail to open logFileStatistics." << std::endl;
		out.close();
	}
	else
	{
		out << "===========================================================================================================" << "\n";
		
		// print table's header
		out << std::setw(iniAttributes.statisticsMaxObjIDSize) << std::setiosflags(std::ios::left) << "objID" << "\t" //STATISTICS_MAX_OBJID_SIZE
			<< std::setw(16) << "texture_number"
			<< std::setw(16) << "xml_number"
			<< std::setw(16) << "total_expected "
			<< std::setw(16) << "total_actual"
			<< std::setw(8) << "status" << "\n";

		std::string tmpObjID;
		long expectedSum = 0;
		long actualSum = 0;

		for (auto var : GetObjStatistic())
		{
			tmpObjID = var.objID;
			if (tmpObjID.size() > iniAttributes.statisticsMaxObjIDSize) //STATISTICS_MAX_OBJID_SIZE
			{
				tmpObjID = tmpObjID.substr(0, iniAttributes.statisticsMaxObjIDSize - 3).append("...");
			}
			out << std::setw(iniAttributes.statisticsMaxObjIDSize) << std::setiosflags(std::ios::left) << tmpObjID << "\t"
				<< std::setw(16) << var.textureNumber
				<< std::setw(16) << var.xmlNumber
				<< std::setw(16) << var.expectedSum
				<< std::setw(16) << var.actualSum
				<< std::setw(8) << (var.isOk ? "ok" : "error") << "\n";

			expectedSum += var.expectedSum;
			actualSum += var.actualSum;

		}
		
		out << "===========================================================================================================" << "\n";
		out << "Expected Total Png: " << expectedSum << "\n"
			<< "Actual   Total Png: " << actualSum << "\n";
		out << (expectedSum == actualSum ? "Rendered successfully!":
			"The sum is incorrect, please check total number of objects respectively!!")<< "\n";
		out << "Consumption of time: "<<statistics.timeConsumption << " minutes."<< "\n";
		out << "===========================================================================================================" << "\n";
	
		out.close();
	}

}
