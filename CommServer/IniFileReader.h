/**
 * This file declare the class IniFileReader, used to read the .ini file.
 */
#ifndef WLA_INI_FILE_H
#define WLA_INI_FILE_H

#include <map>
#include <string>
#include <ostream>
#include "ErrorList.h"

enum EINIFILE_ERR {
	ERR_INIFILE_SUCCESS		= 0,
	ERR_INIFILE_INVAL		= ERR_INIFILE_FIRSTID,
	ERR_INIFILE_OPENFILE,
	ERR_INIFILE_NOSECTION,
	ERR_INIFILE_NOEQUAL,
	ERR_INIFILE_NONAME,
	ERR_INIFILE_NOTFOUND,
	ERR_INIFILE_USEDEFAULTVALUE,

	ERR_INIFILE_LASTID
};

class CIniFileReader
{
public:
	friend std::ostream& operator<<(std::ostream& s, CIniFileReader& m);

public:
	CIniFileReader();
	~CIniFileReader();

	typedef std::map<std::string, std::string> CName2Value;
	typedef std::map<std::string, CName2Value> CSession2NameValue;

	svr_err_t loadConf(const char * conf_file);

	svr_err_t getConfChar(std::string section, std::string name, char& value);
	svr_err_t getConfChar(std::string section, std::string name, char& value, char defaultvalue);
	svr_err_t getConfUchar(std::string section, std::string name, unsigned char& value);
	svr_err_t getConfUchar(std::string section, std::string name, unsigned char& value, unsigned char defaultvalue);
	svr_err_t getConfShort(std::string section, std::string name, short& value);
	svr_err_t getConfShort(std::string section, std::string name, short& value, short defaultvalue);
	svr_err_t getConfUshort(std::string section, std::string name, unsigned short& value);
	svr_err_t getConfUshort(std::string section, std::string name, unsigned short& value, unsigned short defaultvalue);
	svr_err_t getConfInt(std::string section, std::string name, int& value);
	svr_err_t getConfInt(std::string section, std::string name, int& value, int defaultvalue);
	svr_err_t getConfUint(std::string section, std::string name, unsigned int& value);
	svr_err_t getConfUint(std::string section, std::string name, unsigned int& value, unsigned int defaultvalue);
	svr_err_t getConfLong(std::string section, std::string name, long& value);
	svr_err_t getConfLong(std::string section, std::string name, long& value, long defaultvalue);
	svr_err_t getConfUlong(std::string section, std::string name, unsigned long& value);
	svr_err_t getConfUlong(std::string section, std::string name, unsigned long& value, unsigned long defaultvalue);
	svr_err_t getConfStr(std::string section, std::string name, char* value, size_t buf_len);
	svr_err_t getConfStr(std::string section, std::string name, char* value, size_t buf_len, const char* defaultvalue);
	svr_err_t getConfStr(std::string section, std::string name, std::string& value);
	svr_err_t getConfStr(std::string section, std::string name, std::string& value, const std::string& defaultvalue);

protected:
	svr_err_t parseOneLine(std::string& line);
	void trimString(std::string& line);   //清除std::string前后的空白
	void trimQuote(std::string& line);  //清除std::string前后的引号

	CSession2NameValue m_oSession2NameValue;
	std::string m_strCurSection;	
};
extern std::ostream& operator<<(std::ostream& s, CIniFileReader& m);

#endif // WLA_INI_FILE_H
