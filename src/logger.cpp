#include <fstream>
#include <sstream>
#include "logger.hpp"
#include "except.hpp"


// Set default values.
LogLevel Logger::m_msgLevel = LogLevel::info;

bool Logger::m_isPrint = false;
bool Logger::m_isPrintFunctionName = false;
bool Logger::m_isPrintLineNumber = false;
bool Logger::m_isPrintFileName = false;
bool Logger::m_isPrintToFile = false;
std::string Logger::m_fileName = "out.txt";

Logger::Logger(LogLevel level,
               std::string const & functionName,
               std::string lineNumber,
               std::string const & fileName)
{
  static Logger logger;

  if (level >= m_msgLevel)
  {
    m_isPrint = true;

    PrintAdditionalParameters(functionName, lineNumber, fileName);
  }
  else
  {
    m_isPrint = false;
  }
}

void Logger::SetLogLevel(LogLevel const &type)
{
  m_msgLevel = type;
}

void Logger::SetPrintFunctionName(bool const &isPrintFunctionName)
{
  m_isPrintFunctionName = isPrintFunctionName;
}

void Logger::SetPrintLineNumber(bool const & isPrintLineNumber)
{
  m_isPrintLineNumber = isPrintLineNumber;
}

void Logger::SetPrintFileName(bool const & isPrintFileName)
{
  m_isPrintFileName = isPrintFileName;
}

std::string Logger::GetLogLevelAsString(LogLevel const &type)
{
  std::string label;
  switch(type)
  {
    case LogLevel::trace:
      label = "trace";
      break;
    case LogLevel::debug:
      label = "debug";
      break;
    case LogLevel::info:
      label = "info";
      break;
    case LogLevel::warning:
      label = "warning";
      break;
    case LogLevel::error:
      label = "error";
      break;
    case LogLevel::fatal:
      label = "fatal";
      break;
    default:
      throw WrongLogLevelException();
  }
  return label;
}

void Logger::PrintAdditionalParameters(
    std::string const & functionName,
    std::string lineNumber,
    std::string const & fileName)
{
  std::stringstream output;

  output << "[" << GetLogLevelAsString(m_msgLevel) << "] ";

  if (m_isPrintFunctionName)
  {
    output << functionName;
    output << " | ";
  }
  if (m_isPrintLineNumber)
  {
    output << lineNumber;
    output << " | ";
  }
  if (m_isPrintFileName)
  {
    output << fileName;
    output << " | ";
  }

  if (m_isPrintToFile)
  {
    GetFile() << output.str();

    GetFile().flush();
  }
  else
  {
    std::cout << output.str();
  }
}

void Logger::Log(LogLevel const & logLevel,
                std::string const & message,
                std::string const & functionName,
                std::string lineNumber,
                std::string const & fileName)
{
  std::stringstream output;

  if (logLevel >= m_msgLevel)
  {
    PrintAdditionalParameters(
        functionName, lineNumber, fileName);


    if (m_isPrintToFile)
    {
      output << message << std::endl;

      GetFile() << output.str();

      GetFile().flush();
    }
    else
    {
      std::cout << output.str();
    }
  }
}

LogLevel Logger::GetLogLevel()
{
  return m_msgLevel;
}
void Logger::SetPrintToFile(std::string const & fileName,
                            const bool & isPrintToFile)
{
  m_fileName = fileName;
  m_isPrintToFile = isPrintToFile;

  GetFile(true);
}
