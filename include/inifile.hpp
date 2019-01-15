#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>

#ifndef _INIFILE_HPP_
#define _INIFILE_HPP_

class IniFile{
private:
	std::string ini_filename;
	std::vector<std::string> ParamList;
	std::vector<std::string> ValueList;
	
	int KeyVal_Num;

	std::string StrLowCase(std::string& str);
	std::string StrUpperCase(std::string& str);
	std::string GetValue(std::string& param);

	int Str2Int(std::string& str);
	double Str2Double(std::string& str);
	double Str2Boolen(std::string& str);

public:
	IniFile(std::string& inifile);
	~IniFile();

	int Load_IniFile(std::string& ini_filename);

	int GetInt(std::string& param);
	double GetDouble(std::string& param);
	std::string GetString(std::string& param);
};

#endif // _INIFILE_HPP_
