#include "WriteLog.h"
#include <sstream>  
#include <memory>  


using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

CWriteLog* CWriteLog::pInstance = NULL;

#define DO_LOGGER(logLevel, filename, fileline, pFormat, bufSize)\
    log4cplus::Logger _logger = log4cplus::Logger::getRoot();\
    \
    if(_logger.isEnabledFor(logLevel))\
	{                \
		va_list args;            \
		va_start(args, pFormat);        \
		char buf[bufSize] = {0};        \
		vsnprintf(buf, sizeof(buf), pFormat, args);		 \
		va_end(args);										\
		string strFileName = fileName;						\
		string strLastName = fileName;						\
		int nNameStart = strFileName.find_last_of('/');		\
		if (nNameStart > 0)									\
		{\
			strLastName = strFileName.substr(nNameStart + 1);\
		}\
		_logger.forcedLog(logLevel, buf, filename, fileline); \
	}

bool CWriteLog::initialize(const string& strLogFile)
{
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(strLogFile.c_str()));
	Logger logger = Logger::getRoot();
	LOG4CPLUS_INFO(logger, "=======================Log initialize OK!==========================");
	return true;
	//m_oLogger = Logger::getRoot();
}


void CWriteLog::debug(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::DEBUG_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::error(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::ERROR_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::fatal(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::FATAL_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::info(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::INFO_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::warn(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::WARN_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::trace(const char* fileName, const int filelLine, const char* pFormat, ...)
{
	DO_LOGGER(log4cplus::TRACE_LOG_LEVEL, strLastName.c_str(), filelLine, pFormat, 500);
}

void CWriteLog::shutdown()
{
	Logger::getRoot().shutdown();
}

