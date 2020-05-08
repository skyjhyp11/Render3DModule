#include "ObjAnalysis.h"



ObjAnalysis::ObjAnalysis()
{
}


ObjAnalysis::~ObjAnalysis()
{
}
void ObjAnalysis::SetObj(const std::string &objsPath, const std::string &objID)
{
	m_objPath = objsPath + objID + "\\";

	std::vector<std::string> names;
	Help::Getfiles(names, m_objPath);

	for (size_t i = 0; i < names.size(); ++i)
	{
		if (!strcmp("obj", GotFileTypeByPostfix(names[i]).c_str()))
		{
			m_objPathID = m_objPath + names[i];
		}
		else if (!strcmp("mtl", GotFileTypeByPostfix(names[i]).c_str()))
		{
			m_mtl = names[i];
		}
		else//Attention: only support one texture files
		{
			m_texture = names[i];
			m_texturePathID = m_objPath + names[i];
		}
	}
}

std::string ObjAnalysis::GetObjPathID()
{
	return m_objPathID;
}

std::string ObjAnalysis::GetObjPath()
{
	return m_objPath;
}

std::string ObjAnalysis::GetTexturePathID()
{
	return m_texturePathID;
}

void ObjAnalysis::Calculate()
{
	std::ifstream infile;
	infile.open(m_objPathID.data());
	assert(infile.is_open());

	std::string oneLine;
	std::vector<std::string> splitedLine;
	//splitedLine[0]=("0");
	size_t partsCount;
	size_t readCount = 0;
	float tmpX, tmpY, tmpZ;

	while (getline(infile, oneLine))
	{
		splitedLine.clear();
		partsCount = Help::StringSplit(splitedLine, oneLine, " ");
		if (partsCount == 4 && (splitedLine[0] == "v" || splitedLine[0] == "V"))
		{
			++readCount;
			if (1 == readCount)
			{
				m_maxX = (float)atof(splitedLine[1].c_str());
				m_maxY = (float)atof(splitedLine[2].c_str());
				m_maxZ = (float)atof(splitedLine[3].c_str());
				m_minX = m_maxX;
				m_minY = m_maxY;
				m_minZ = m_maxZ;
				continue;
			}
			tmpX = (float)atof(splitedLine[1].c_str());
			tmpY = (float)atof(splitedLine[2].c_str());
			tmpZ = (float)atof(splitedLine[3].c_str());

			m_maxX = m_maxX > tmpX ? m_maxX : tmpX;
			m_minX = m_minX < tmpX ? m_minX : tmpX;
			m_maxY = m_maxY > tmpY ? m_maxY : tmpY;
			m_minY = m_minY < tmpY ? m_minY : tmpY;
			m_maxZ = m_maxZ > tmpZ ? m_maxZ : tmpZ;
			m_minZ = m_minZ < tmpZ ? m_minZ : tmpZ;
		}
		// line "vn" or line "vt" must be after line "v", otherwise the data will be wrong.
		//else if (partsCount > 0 && (splitedLine[0] == "vn" || splitedLine[0] == "vt"))
		else if (partsCount > 0 && (splitedLine[0] == "f")) //TBD
		{
			break;
		}
	}

	float ax = (m_maxX + m_minX) / 2;
	float ay = (m_maxY + m_minY) / 2;
	float az = (m_maxZ + m_minZ) / 2;
	m_shiftX = 0 - ax;
	m_shiftY = 0 - ay;
	m_shiftZ = 0 - az;

	float dx = m_maxX - m_minX;
	float dy = m_maxY - m_minY;
	float dz = m_maxZ - m_minZ;
	m_scale = (float)(iniAttributes.objBaseScale / std::fabs(std::max(std::max(dx, dy), dz))); //OBJ_BASE_SCALE

	infile.close();
}

float ObjAnalysis::GetShiftX()
{
	return m_shiftX;
}

float ObjAnalysis::GetShiftY()
{
	return m_shiftY;
}

float ObjAnalysis::GetShiftZ()
{
	return m_shiftZ;
}

float ObjAnalysis::GetScale()
{
	return m_scale;
}

std::string ObjAnalysis::GotFileTypeByPostfix(std::string filename)
{
	std::vector<std::string> splitStrs;
	Help::StringSplit(splitStrs, filename, ".");
	return splitStrs[splitStrs.size() - 1];
}
