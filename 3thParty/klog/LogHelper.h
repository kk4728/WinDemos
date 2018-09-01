#ifndef LOG_HELPER_H_
#define LOG_HELPER_H_

#include <windows.h>

class LogHelper {
public:
	enum LEVEL { //INFO,WARNING, ERROR, �� FATAL
		INFO = 0,
		WARNING,
		ERR ,
		FATAL,
		DBG
	};
public:
	LogHelper();
	LogHelper(const char* path);

	~LogHelper();

	static LogHelper& GetInstance(); //����ģʽ

	bool Init();
	void SetLogFile(const char* name);
	const char* GetLogFile() const { return m_logName; }
	const char* GetLogDir() const { return m_logDir;  }

	//void LogMsg(const wchar_t* pFormat, ...); //���⣬ wchar_t �� char����ͬʱ��
	void LogMsg(const wchar_t* pFormat, ...);
	void LogMsg(const char* pFormat, ...);
	LogHelper& Log(LEVEL level);

	LogHelper& operator<<(const char* pStr);
	LogHelper& operator<<(const wchar_t* pStr);
	//LogHelper& operator<<(const CString& str);
	LogHelper& operator<<(int n);
	

	void Flush();

protected:
	bool IsDayNew();
	bool WriteBuffer(const char* pStr, ...);

private:
	CRITICAL_SECTION	m_cs;
	FILE*				m_pFile;
	char				m_logName[256];
	char				m_logDir[256];
	int					m_curYear;
	int					m_curMonth;
	int					m_curDay;
	char				m_buffer[1024];
	bool				m_bBufBusy;
	LEVEL				m_curLevel;

	//��ֹ��ֵ���졢���ƹ���
	LogHelper(const LogHelper&) {}
	LogHelper& operator=(const LogHelper&) {
		return *this;
	}
};





#include <FSTREAM>
#include <string>
using std::ofstream;
using std::string;
class LogHelper2 : public ofstream 
{
public:
	static LogHelper2* GetInstance();

	bool Init();
	bool IsDayNew();
	const char* GetLogName() const ;
	const char* GetLogDir() const;


private:
	int curDay;
	int curMonth;
	int curYear;

	string logName;
	string logDir;

	LogHelper2(const LogHelper2&) { }
	LogHelper2& operator=(const LogHelper2&) { }

};

#endif //LOG_HELPER_H_
