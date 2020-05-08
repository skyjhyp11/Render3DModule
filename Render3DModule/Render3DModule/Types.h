#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>
#include <set>
#define TRUE  1
#define FALSE 0
typedef unsigned int     size_t;

// define the values calculated by ObjAnalysis.
// need to use in XmlCreator.
struct  ParameterXml
{
	struct  Translate
	{// the same object should have the same shift to screen center
		float x;
		float y;
		float z;
	} Trans;
	struct  Rotate
	{// the same object should have the same shift to screen center
		int x;
		int y;
		int z;
	} Rotat;

	float baseObjScale;
};

// define the values using to modify in loop.
// need to use in XmlCreator.
struct  ParameterXmlModify
{
	int index;
	struct  Rotate
	{
		int x;
		int y;
		int z;
	} Rotat;
	float objScale;
	float emitScale;
	float emitPointPositionX;

};

struct  ObjStatistic
{
	std::string objID;
	size_t textureNumber;
	size_t xmlNumber;
	size_t expectedSum;
	size_t actualSum;
	bool isOk;

	ObjStatistic() :objID(" ? "), textureNumber(0), xmlNumber(0),
		expectedSum(0), actualSum(0), isOk(false)
	{}

};

struct IniAttributes
{
	bool createMaskModule;
	bool debugLogDetails;
	bool performanceNeedSleep;
	bool debugIncreaseXml;
	bool debugDeleteExrFile;
	size_t performanceSleepSecond;
	size_t xmlBatchCount;
	size_t exrBatchCount;
	size_t statisticsMaxObjIDSize;
	int stepAngle;
	float stepScale;
	float objBaseScale;
	float stepEmitter;


	//the original value is recommended
	IniAttributes() :createMaskModule(false),
		debugLogDetails(true),
		performanceNeedSleep(true),
		debugIncreaseXml(false),
		debugDeleteExrFile(false),
		performanceSleepSecond(15),
		xmlBatchCount(4),
		exrBatchCount(16),
		statisticsMaxObjIDSize(32),
		stepAngle(10),
		stepScale(float(0.05)),
		objBaseScale(float(3.0)),
		stepEmitter(float(0.5))
	{}
};

struct Roi
{
	size_t leftTopX;
	size_t leftTopY;
	size_t rightBottomX;
	size_t rightBottomY;
};

class ObjBaseInfo
{
public:
	ObjBaseInfo() :
		objID("UnknownObjID"),
		Roi{ 0,0,0,0 },
		baseInfo{ 0,0,0,1,0,0,0 }
	{}

	virtual ~ObjBaseInfo() {};

public:
	std::string objID;
	Roi Roi;
	struct BaseInfo
	{
		int objBaseRotateX;
		int objBaseRotateY;
		int objBaseRotateZ;
		float objBaseScale;
		float objBaseTranslateX;
		float objBaseTranslateY;
		float objBaseTranslateZ;
	}baseInfo;

	bool operator<(const ObjBaseInfo & objBaseInfo) const
	{
		return objID < objBaseInfo.objID;
	}
};
class ObjBaseInfoFinder
{
public:
	ObjBaseInfoFinder(const std::string objID) :m_objID(objID) {}
	bool operator ()(const std::set<ObjBaseInfo>::value_type &value)
	{
		return value.objID == m_objID;
	}
private:
	std::string m_objID;
};

struct StatisticsSummarizeParam
{
	double timeConsumption;
}; 
#endif /*__TYPES_H__*/