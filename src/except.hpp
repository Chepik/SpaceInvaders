#pragma once

#include <iostream>
#include <exception>
#include <string>


class NotImplementedException : public std::exception
{
public:
  virtual const char* what() const noexcept;
};

class WrongLogLevelException : public std::exception
{
public:
  virtual const char* what() const noexcept;
};

class LoadImagesException : public std::exception
{
public:
  LoadImagesException(std::string const & fileName) {
    m_message = "Can't load an image! Image path: " + fileName;
  }

  virtual const char * what() const noexcept;

private:
  std::string m_message;
};

class ReadSettingsException : public std::exception
{
public:
  ReadSettingsException(std::string const & fileName) {
    m_message = "Can't read settings from a file! File path: "
        + fileName;
  }

  virtual const char * what() const noexcept;

private:
  std::string m_message;
};

class InitialiseGameException : public std::exception
{
public:
  InitialiseGameException() = default;

  virtual const char * what() const noexcept;
};

class ReadFileException : public std::exception
{
public:
  ReadFileException(std::string const & fileName) {
    m_message = "Can't read a file! File path: "
        + fileName;
  }

  virtual const char * what() const noexcept;

private:
  std::string m_message;
};

class WriteFileException : public std::exception
{
public:
  WriteFileException(std::string const & fileName) {
    m_message = "Can't write to a file! File path: "
        + fileName;
  }

  virtual const char * what() const noexcept;

private:
  std::string m_message;
};

class WrongLevelException : public std::exception
{
 public:
  WrongLevelException(size_t level)
  {
    m_message = "Wrong level number. Level number: " + std::to_string(level);
  }

  virtual const char * what() const noexcept;

 private:
  std::string m_message;
};
