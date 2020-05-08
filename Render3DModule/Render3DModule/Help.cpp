#include "Help.h"
#include <typeinfo>

#ifdef __WINDOWS__
#include <windows.h>
#else

#endif // !__WINDOWS__




Help::Help()
{
}


Help::~Help()
{
}


int Help::StringSplit(std::vector<std::string> &dst, const std::string &src, const std::string &separator)
{
	if (src.empty() || separator.empty())
		return 0;

	int nCount = 0;
	std::string temp;
	size_t pos = 0, offset = 0;

	//  split NO.1~n-1
	while ((pos = src.find_first_of(separator, offset)) != std::string::npos)
	{
		temp = src.substr(offset, pos - offset);
		if (temp.length() > 0) {
			dst.push_back(temp);
			nCount++;
		}
		offset = pos + 1;
	}

	// split NO.n
	temp = src.substr(offset, src.length() - offset);
	if (temp.length() > 0) {
		dst.push_back(temp);
		nCount++;
	}

	return nCount;
}

bool Help::FileCopy(std::string &srcFile, std::string &dstFile, bool needOverwrite)
{
	std::ifstream in;
	std::ofstream out;
	in.open(srcFile, std::ios::binary);
	if (in.fail())
	{
		std::cout << "ERROR: Fail to open the source file : " << srcFile << std::endl;

		my_logger->error("  [{}]: open src file failed in function {} line {} !!!", srcFile, __FUNCTION__, __LINE__);
		my_logger->flush();

		in.close();
		out.close();
		assert(FALSE);
		return FALSE;
	}
	// if this file not exist, create a new one.
	if (0 != _access(dstFile.c_str(), 0))
	{
		out.open(dstFile, std::ios::binary);
		if (out.fail())
		{
			std::cout << "ERROR: Fail to create the new file." << std::endl;
			my_logger->error("  [{}]: create dst file failed in function {} line {} !!!", srcFile, __FUNCTION__, __LINE__);
			out.close();
			in.close();
			return FALSE;
		}
		else
		{
			out << in.rdbuf();
			out.close();
			in.close();
			return TRUE;
		}
	}
	//file exist
	else
	{
		if (needOverwrite)
		{
			// std::ios::trunc | out 都不能将原始文件清空,再写入,所以使用系统命令进行删除
			std::string cmd = "del " + dstFile;
			system(cmd.c_str());

			out.open(dstFile, std::ios::trunc | std::ios::binary);
			if (out.fail())
			{
				std::cout << "ERROR: Fail to overwrite src file." << std::endl;
				my_logger->error("  [{}]: overwrite src file failed in function {} line {} !!!", srcFile, __FUNCTION__, __LINE__);
				out.close();
				in.close();
				return FALSE;
			}
			else
			{
				out << in.rdbuf();
				out.close();
				in.close();
				return TRUE;
			}
		}
	}
	return FALSE;
}

void Help::Getfiles(std::vector<std::string> &outfileNames, const std::string & path, bool getDirectory, bool getFullName)
{
	std::vector<std::string> fileNames;

	//file handler
	long hFile = 0;
	struct _finddata_t fileinfo;
	std::string tmpExpress;

	//if .append("*.exe"), means only search exe files in path. 
	if ((hFile = _findfirst(tmpExpress.assign(path).append("*").c_str(), &fileinfo)) != -1)
	{
		// only get directories
		if (getDirectory)
		{
			do {
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					{
						if (!getFullName)	fileNames.push_back(fileinfo.name);
						else				fileNames.push_back(path + fileinfo.name);
					}
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		// only get files except directories
		else
		{
			do {
				if (!(fileinfo.attrib & _A_SUBDIR))
				{
					if (!getFullName)	fileNames.push_back(fileinfo.name);
					else				fileNames.push_back(path + fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	outfileNames = fileNames;
}

void Help::GetSpecifiedTypefiles(std::vector<std::string>& outfileNames, const std::string & path, const std::string & fileType, bool getFullName)
{
	std::vector<std::string> fileNames;

	//file handler
	long hFile = 0;
	struct _finddata_t fileinfo;
	std::string tmpExpress;

	if ((hFile = _findfirst(tmpExpress.assign(path).append("*.").append(fileType).c_str(), &fileinfo)) != -1)
	{
		do {
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				if (!getFullName)	fileNames.push_back(fileinfo.name);
				else				fileNames.push_back(path + fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	outfileNames = fileNames;

}

int Help::CreateFolder(const std::string &folderName)
{
	if (0 != _access(folderName.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		_mkdir(folderName.c_str());
		return 1;
	}
	else
	{
		//my_logger->info("     folder already exists: {}", folderName);
		std::cout << "INFO: folder already exists: " << folderName << std::endl;
		return 0;
	}
}

void Help::ClearFolder(const std::string & folderName)
{
	std::string cmd;
	cmd = "del /q " + folderName + "*";
	system(cmd.c_str());
}

std::string Help::GetExePathByWinAPI()
{
#ifdef __WINDOWS__
	std::string result;
	wchar_t path[100];
	::GetModuleFileName(NULL, path, sizeof(path));
	result = ConvertWCharToStdString(path);

	return result.substr(0, result.find_last_of("\\"));
#else
	//TO-DO: waiting for implementation
	return std::string("NO EXE PATH!!!");
#endif

}

//test ok.
std::string Help::ConvertWCharToStdString(const wchar_t *wStr)
{
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, wStr, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	::WideCharToMultiByte(CP_ACP, 0, wStr, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

std::string Help::ConvertStdWStringToStdString(std::wstring & inputws)
{
	return Help::ConvertWCharToStdString(inputws.c_str());
}

//Converting a Ansi string to WChar string  
std::wstring Help::ConvertStdStringToStdWString(std::string srcString)
{
	std::string srcStr(srcString);
	int nSize = MultiByteToWideChar(CP_ACP, 0, srcStr.c_str(), srcStr.size(), 0, 0);
	if (nSize <= 0) return NULL;

	wchar_t *pwszDst = new wchar_t[nSize + 1];
	if (NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0, srcStr.c_str(), srcStr.size(), pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF)
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	std::wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

//测试:加 static 后,ok
//缺点: tmpWstring对象会一直在静态成员区存着,直到程序结束
const wchar_t * Help::ConvertStdStringToWChar(std::string srcString)
{
	//若不定义成static, tmpWstring对象随函数结束而消失, 返回值(指针类型)为未定值!!!!
	static std::wstring tmpWstring = ConvertStdStringToStdWString(srcString);
	return tmpWstring.c_str();
}

std::string Help::ReadFileIntoString(const std::string &filename)
{
	std::ifstream infile(filename.c_str());
	std::ostringstream buffer;
	char ch;

	while (buffer && infile.get(ch))
		buffer.put(ch);

	return buffer.str();
}
