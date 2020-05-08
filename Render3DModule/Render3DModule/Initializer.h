#pragma once
#include <set>
#include "Help.h"
#include "Config.h"

class Initializer
{
public:
	Initializer();
	virtual ~Initializer();

	bool ReadIniFile();
	bool ReadIniFileAttributes();
	bool ReadIniFileObjsBaseInfo();

	IniAttributes GetIniAttributes();
	std::set<ObjBaseInfo> &GetIniObjsBaseInfo();

private:
	std::string GetIniFileDefaultSetting();
	bool SetIniAttributesByWinAPI();
	bool SetIniAttributesByBoost();
	bool SetIniObjBaseInfoByBoost();


private:
	std::string m_iniAttributesFile;
	std::string m_iniObjBaseInfoFile;

	IniAttributes m_iniAttributes;
	std::set<ObjBaseInfo> m_objsBaseInfo;
};

