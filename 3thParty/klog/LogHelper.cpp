#include "StdAfx.h"
#include "LogHelper.h"
#include <share.h>


#pragma warning(disable:4996)


const char StartMsg[] = "\r\n-------------------------------- SCM200 logging starts right here --------------------------------\n";
const char StopMsg[]  = "-------------------------------- SCM200 logging stops right here. --------------------------------\n\n";

static const char* s_levelStr[] = {
	" INFO  ",
	"WARNING",
	" ERROR ",
	" FATAL ",
	" DEBUG "
};

LogHelper::LogHelper() 
{
	m_curDay = m_curMonth = m_curYear = 0;
	InitializeCriticalSection(&m_cs);

	memset(m_logDir, sizeof(m_logDir), 0x00);
	memset(m_logName, sizeof(m_logName), 0x00);
	strcpy(m_logDir, "C:\\Program Files\\SCM200\\logs\\");
}

LogHelper::LogHelper(const char* path) {
	m_curDay = m_curMonth = m_curYear = 0;
	InitializeCriticalSection(&m_cs);

	memset(m_logDir, sizeof(m_logDir), 0x00);
	memset(m_logName, sizeof(m_logName), 0x00);
	strcpy(m_logDir, path);
}

LogHelper::~LogHelper() {
	if(m_pFile) {
		if(m_pFile)
			fwrite(StopMsg, 1, sizeof(StopMsg)-1, m_pFile);
		fclose(m_pFile);
	}

	DeleteCriticalSection(&m_cs);
}

bool LogHelper::Init() {
	char buf[256] = {0};
	char path[256] = {0};

	memset(m_logName, 0x00, sizeof(m_logDir));

	//获取系统当前时间
	SYSTEMTIME t;
	GetLocalTime(&t);
	m_curYear = t.wYear;
	m_curMonth = t.wMonth;
	m_curDay = t.wDay;
	sprintf(buf, "SCM200 %04d-%02d-%02d.log", m_curYear, m_curMonth, m_curDay);
	strcpy(m_logName, buf);
	strcpy(path, m_logDir);
	strcat(path, buf);

	TCHAR cn[MAX_PATH] = _T("");
	unsigned long len;
	//gethostname(t, 100);
	GetComputerName(cn, &len);
	//GetUserName(t1, l);
	//
	// Open file without write sharing
	m_pFile=_fsopen(path, "aS", _SH_DENYWR); //禁止写共享
	if(!m_pFile)
		return false;
	
	fwrite(StartMsg, 1, sizeof(StartMsg)-1, m_pFile);
	
	//const char ch[] = "121212";
	//int n = fwrite(ch, 1, sizeof(ch), m_pFile);

	return true;
}

bool LogHelper::IsDayNew() {
	SYSTEMTIME t;
	GetLocalTime(&t);
	
	if( t.wDay > m_curDay ||
		t.wMonth > m_curMonth || 
		t.wYear>m_curYear ) { //说明出现新的一天
		m_curYear = t.wYear;
		m_curMonth = t.wMonth;
		m_curDay = t.wDay;

		//
		if(m_pFile) { // 写入结束信息
			if(m_pFile)
				fwrite(StopMsg, sizeof(char), sizeof(StopMsg)-1, m_pFile);
			fflush(m_pFile);
			fclose(m_pFile);
		}
		
		return Init();	
	}

	return false;
}

void LogHelper::LogMsg(const wchar_t* pFormat, ...)
{
	wchar_t    cbuf[256] = { 0 };
	char	   tbuf[256] = {0};
	SYSTEMTIME t;
	int        cnt;
	va_list    args;
	
	IsDayNew(); 
	
	EnterCriticalSection(&m_cs);
	if(m_pFile)
	{
		GetLocalTime(&t);
		
//#if defined _UNICODE || UNICODE
		cnt = wsprintf(cbuf, _T("[%04d-%02d-%02d %02d:%02d:%02d.%03d] [tid=0x%p] "),
			t.wYear, t.wMonth, t.wDay, 
			t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,
			GetCurrentThread());
		WideCharToMultiByte(CP_ACP, 0, cbuf, wcslen(cbuf) + 1, tbuf, 256, NULL, NULL);
		fwrite(tbuf, sizeof(char), strlen(tbuf), m_pFile);
		
		va_start(args,pFormat);
		cnt = wvsprintf(cbuf,pFormat,args)  ;	
		memset(tbuf, 0x00, 256);
		WideCharToMultiByte(CP_ACP, 0, cbuf, wcslen(cbuf) + 1, tbuf, 256, NULL, NULL);
		fwrite(tbuf, sizeof(char), strlen(tbuf), m_pFile);		
	}
	LeaveCriticalSection(&m_cs);
}

void LogHelper::LogMsg(const char* pFormat, ...)
{
	char       cbuf[256] = { 0 };
	SYSTEMTIME t;
	int        cnt;
	va_list    args;
	
	IsDayNew(); 
	
	EnterCriticalSection(&m_cs);
	if(m_pFile)
	{
		//GetLocalTime(&t);
		
		//cnt = sprintf(cbuf, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] [tid=0x%p] ",
		//	t.wYear, t.wMonth, t.wDay, 
		//	t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,
		//	GetCurrentThread());
		//fwrite(cbuf, sizeof(char), cnt, m_pFile);
		
		va_start(args,pFormat);
		cnt = vsprintf(cbuf,pFormat,args)  ;	
		fwrite(cbuf, sizeof(char), cnt, m_pFile);		
	}
	LeaveCriticalSection(&m_cs);
}

void LogHelper::Flush() {
	if(m_pFile) {
		EnterCriticalSection(&m_cs);
		fflush(m_pFile);
		LeaveCriticalSection(&m_cs);
	}
}

bool LogHelper:: WriteBuffer(const char* pFormat, ...) {
	char       cbuf[256] = { 0 };
	SYSTEMTIME t;
	int        cnt;
	va_list    args;

	IsDayNew();

	if(!strlen(pFormat)) {
		fwrite("\n", sizeof(char), 1, m_pFile);
		return true;
	}

	EnterCriticalSection(&m_cs);
	if (m_pFile)
	{
		if (strlen(m_buffer) == 0) {
			GetLocalTime(&t);
			cnt = sprintf(cbuf, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] [tid=%04d] [%s]",
				t.wYear, t.wMonth, t.wDay,
				t.wHour, t.wMinute, t.wSecond, t.wMilliseconds,
				GetCurrentThreadId(),
				s_levelStr[static_cast<int>(m_curLevel)]
			);
			strcpy(m_buffer, cbuf);
		}

		if (strlen(m_buffer) > 0) {
			va_start(args, pFormat);
			memset(cbuf, 0x00, sizeof(cbuf));
			cnt = vsprintf(cbuf, pFormat, args);
			strcat(m_buffer, cbuf);
		}
		

		if (strpbrk(cbuf, "\r\n")) {
			fwrite(m_buffer, sizeof(char), strlen(m_buffer), m_pFile);
			//fflush(m_pFile); //可能影响效率

			memset(m_buffer, 0x00, sizeof(m_buffer)); // clear
		}
	}
	LeaveCriticalSection(&m_cs);

	return true;
}
LogHelper& LogHelper::Log(LEVEL level) {
	switch (level)
	{
	case LogHelper::INFO:		
	case LogHelper::WARNING:
	case LogHelper::ERR:
	case LogHelper::FATAL:
		m_curLevel = level;
	default:
		break;
	}
	return *this;
}

LogHelper& LogHelper::operator<<(const char* pStr) {
	//LogMsg(pStr);
	WriteBuffer(pStr);
	return *this;
}

LogHelper& LogHelper::operator<<(const wchar_t* pStr) {
	//LogMsg(pStr);
	char tbuf[1024];
	ZeroMemory(tbuf, 1024);
	WideCharToMultiByte(CP_ACP, 0, pStr, wcslen(pStr) + 1, tbuf, 1024, NULL, NULL);

	WriteBuffer(tbuf);
	return *this;
}

//LogHelper& operator<<(const CString& str);
LogHelper& LogHelper::operator<<(int n) {
	//LogMsg("%d", n);
	WriteBuffer("%d",n);
	return *this;
}




