#ifndef _LOG_H_  
#define _LOG_H_  

#include <string> 
#include <stdarg.h>
#include <log4cplus/logger.h>  
#include <log4cplus/loggingmacros.h>  
#include <log4cplus/fileappender.h>  
#include <log4cplus/configurator.h>  
#include <log4cplus/loggingmacros.h> 
  


//#define DO_LOGGER2(logLevel, pFormat,...)										\
{																	\
    log4cplus::Logger _logger = log4cplus::Logger::getRoot();		\
    if(_logger.isEnabledFor(logLevel))\
{                \
		char buf[500] = { 0 };											\
		snprintf(buf, sizeof(buf), pFormat, ##__VA_ARGS__);				\
_logger.forcedLog(logLevel, buf, "test", 111); \
}\
};
//#define WARNI log4cplus::DEBUG_LOG_LEVEL

//#define DO_LOGGER_OBJ(logLevel, pFormat)										\
{																	\
	log4cplus::Logger _logger = log4cplus::Logger::getRoot();		\
    if(_logger.isEnabledFor(logLevel))\
{                \
		_logger.forcedLog(logLevel, pFormat);           \
}\
};

#define LogWriter					CWriteLog::getInstance()

#define LOG_INIT(logConfigFile)		LogWriter.initialize(logConfigFile)
#define LOG_DEBUG(...)				LogWriter.debug(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)				LogWriter.error(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_FATAL(...)				LogWriter.fatal(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_INFO(...)				LogWriter.info(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_WARN(...)				LogWriter.warn(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_TRACE(...)				LogWriter.trace(__FILE__, __LINE__, __VA_ARGS__);
#define LOG_EDN						LogWriter.shutdown();


class CWriteLog
{
public:

	static CWriteLog& getInstance()
	{
		if (NULL == pInstance)
		{
			pInstance = new CWriteLog();
		}
		return *pInstance;
	};

	bool initialize(const std::string& strLogFile);

	void debug(const char* fileName, const int filelLine, const char* pFormat, ...);
	void error(const char* fileName, const int filelLine, const char* pFormat, ...);
	void fatal(const char* fileName, const int filelLine, const char* pFormat, ...);
	void info(const char* fileName, const int filelLine, const char* pFormat, ...);
	void warn(const char* fileName, const int filelLine, const char* pFormat, ...);
	void trace(const char* fileName, const int filelLine, const char* pFormat, ...);

	void shutdown();

private:
	static CWriteLog* pInstance;
	//log4cplus::Logger m_oLogger;
};

#endif  