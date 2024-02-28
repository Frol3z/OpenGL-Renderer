#include <iostream>

class Logger
{
public:
	enum Level
	{
		LEVEL_ERROR = 0,
		LEVEL_WARNING,
		LEVEL_INFO
	};

public:
	Logger()
		: m_LogLevel(LEVEL_ERROR)
	{
	}

	void SetLevel(Level level)
	{
		m_LogLevel = level;
	}

	void Error(const std::string& message) const
	{
		if(m_LogLevel >= LEVEL_ERROR)
			std::cout << "[ERROR] " << message << std::endl;
	}
	void Warning(const std::string& message) const
	{
		if (m_LogLevel >= LEVEL_WARNING)
			std::cout << "[WARNING] " << message << std::endl;
	}
	void Info(const std::string& message) const
	{
		if (m_LogLevel >= LEVEL_INFO)
			std::cout << "[INFO] " << message << std::endl;
	}

private:
	Level m_LogLevel;
};