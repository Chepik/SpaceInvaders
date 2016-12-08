#include "except.hpp"


const char* NotImplementedException::what() const noexcept
{
  return "Not implemented!";
}

const char* WrongLogLevelException::what() const noexcept
{
  return "Wrong log level!";
}

const char * LoadImagesException::what() const noexcept
{
  return m_message.c_str();
}

const char * ReadSettingsException::what() const noexcept
{
  return m_message.c_str();
}

const char * InitialiseGameException::what() const noexcept
{
  return "Can't initialise the game!";
}

const char * ReadFileException::what() const noexcept
{
  return m_message.c_str();
}

const char *WriteFileException::what() const noexcept
{
  return m_message.c_str();
}
const char * WrongLevelException::what() const noexcept
{
  return m_message.c_str();
}
