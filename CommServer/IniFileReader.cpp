
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "IniFileReader.h"

using namespace std;

CIniFileReader::CIniFileReader()
{
}

CIniFileReader::~CIniFileReader()
{
}

int CIniFileReader::loadConf(const char * conf_file)
{
	m_strCurSection = "";
	
	ifstream fin(conf_file);
	if(!fin.is_open()) 
	{
		return ERR_INIFILE_OPENFILE;
	}

	int line_count = 0;

	while(!fin.eof()) 
	{
		//读取一行配置
		string inbuf, subbuf;
		getline(fin, inbuf, '\n');
		line_count ++;
		
		size_t last_pos = inbuf.find_last_of("\r\n");
		if (last_pos != (int)string::npos) {
			subbuf = inbuf.substr(0, last_pos);
		}
		else {
			subbuf = inbuf;
		}
		
		//解析一行配置
		svr_err_t nRet;
		if ((nRet = parseOneLine(subbuf)) != ERR_INIFILE_SUCCESS) 
		{
			return line_count;
		}
	}
	return ERR_INIFILE_SUCCESS;
}

void CIniFileReader::trimString(string& line)
{
	size_t first= line.find_first_not_of(" \t\r\n");
	size_t last = line.find_last_not_of(" \t\r\n");
	if(first == (int)string::npos || last == (int)string::npos) 
	{
		//为空行
		line="";
	} else {
		line = line.substr(first, last-first+1);
	}
}

void CIniFileReader::trimQuote(string& line)
{
	size_t first= line.find_first_not_of("'\"");
	size_t last = line.find_last_not_of("'\"");
	if(first == (int)string::npos || last == (int)string::npos) {
		//为空行
		line="";
	} else {
		line = line.substr(first, last-first+1);
	}
}

svr_err_t CIniFileReader::parseOneLine(string& line)
{
	size_t first = 0, last = 0;

	trimString(line);
	if (line.empty()) {
		return ERR_INIFILE_SUCCESS;
	}  
	//分析是否为注释行, 以#或;为注释开始
	if (line[0] == '#' || line[0] == ';') {
		return ERR_INIFILE_SUCCESS;
	}
//	first = line.find_first_of("#;"); 
//	if (first != (int)string::npos) {
//		line = line.substr(0, first);
//	}
//	if (line.empty()) {
//		return ERR_WLA_INIFILE_SUCCESS;
//	}  

	//分析是否为section 
	last = line.rfind(']');
	if( line[0] == '[' && last != (int)string::npos) {
		m_strCurSection = line.substr(1,last-1);
		CName2Value empty_map;
		m_oSession2NameValue.insert(CSession2NameValue::value_type(m_strCurSection, empty_map));
		return ERR_INIFILE_SUCCESS;
	}
   
	//为一正常配置行
	if(m_strCurSection.empty()) {
		return ERR_INIFILE_NOSECTION;
	}
	
	if((first = line.find('=')) == (int)string::npos) {
		//没有= 符号
		return ERR_INIFILE_NOEQUAL;
	}

	string name = line.substr(0,first);
	string value = line.substr(first+1, string::npos);

	trimString(name);
	trimString(value);
	trimQuote(value);
	
	if (name.empty()) {
		//name为空
		return ERR_INIFILE_NONAME;		
	}
	
	CName2Value& name_value_map = m_oSession2NameValue[m_strCurSection];
	name_value_map.insert(CName2Value::value_type(name, value));
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfChar(string section, string name, char& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = (char)strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfChar(string section, string name, char& value, char defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = (char)strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}


svr_err_t CIniFileReader::getConfUchar(string section, string name, unsigned char& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = (char)strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUchar(string section, string name, unsigned char& value, unsigned char defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = (unsigned char)strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}


svr_err_t CIniFileReader::getConfShort(string section, string name, short& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = (short)strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfShort(string section, string name, short& value, short defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = (short)strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUshort(string section, string name, unsigned short& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = (unsigned short)strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUshort(string section, string name, unsigned short& value, unsigned short defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = (unsigned short)strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfInt(string section, string name, int& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfInt(string section, string name, int& value, int defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUint(string section, string name, unsigned int& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUint(string section, string name, unsigned int& value, unsigned int defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfLong(string section, string name, long& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfLong(string section, string name, long& value, long defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = strtol(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUlong(string section, string name, unsigned long& value)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	value = strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfUlong(string section, string name, unsigned long& value, unsigned long defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = strtoul(str_value.c_str(), NULL, 0);
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfStr(string section, string name, string& value)
{
	CSession2NameValue::iterator map_pos = m_oSession2NameValue.find(section);
	if (map_pos == m_oSession2NameValue.end()) {
		return ERR_INIFILE_NOTFOUND;
	}
	CName2Value::iterator value_pos = map_pos->second.find(name);
	if (value_pos == map_pos->second.end()) { 
		return ERR_INIFILE_NOTFOUND;
	}
	value = value_pos->second;
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfStr(string section, string name, string& value, const string& defaultvalue)
{
	CSession2NameValue::iterator map_pos = m_oSession2NameValue.find(section);
	if (map_pos == m_oSession2NameValue.end()) {
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	CName2Value::iterator value_pos = map_pos->second.find(name);
	if (value_pos == map_pos->second.end()) { 
		value = defaultvalue;
		return ERR_INIFILE_USEDEFAULTVALUE;
	}
	value = value_pos->second;
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfStr(string section, string name, char* value, size_t buf_len)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		return nRet;
	}
	
	if (buf_len < str_value.length() + 1) {
		return ERR_INIFILE_INVAL;
	}

	memcpy(value, str_value.c_str(), str_value.length());
	value[buf_len -1] = 0;
	value[str_value.length()] = 0;
	return ERR_INIFILE_SUCCESS;
}

svr_err_t CIniFileReader::getConfStr(string section, string name, char* value, size_t buf_len, const char* defaultvalue)
{
	string str_value;
	svr_err_t nRet;
	if ((nRet = getConfStr(section, name, str_value)) != ERR_INIFILE_SUCCESS) {
		size_t len = strlen(defaultvalue);
		if (buf_len < len + 1) {
			return ERR_INIFILE_INVAL;
		}

		memcpy(value, defaultvalue, len);
		value[buf_len -1] = 0;
		value[len] = 0;

		return ERR_INIFILE_USEDEFAULTVALUE;
	}

	if (buf_len < str_value.length() + 1) {
		return ERR_INIFILE_INVAL;
	}

	memcpy(value, str_value.c_str(), str_value.length());
	value[buf_len -1] = 0;
	value[str_value.length()] = 0;
	return ERR_INIFILE_SUCCESS;
}

ostream& operator<<(ostream& s, CIniFileReader& m)
{
	CIniFileReader::CSession2NameValue::iterator map_pos;
	for (map_pos = m.m_oSession2NameValue.begin(); map_pos != m.m_oSession2NameValue.end(); map_pos++) {
		s << "[" << map_pos->first << "]" << endl;
		CIniFileReader::CName2Value::iterator value_pos = map_pos->second.begin();
		for (; value_pos != map_pos->second.end(); value_pos++) {
			s << value_pos->first << " = " << value_pos->second << endl;
		}
	}
	return s;
}
