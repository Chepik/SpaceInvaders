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

class CantLoadImagesException : public std::exception
{
public:
  CantLoadImagesException(std::string const & message) {
    m_message = "Can't load an image! Image path: " + message;
  }

  virtual const char * what() const noexcept;

private:
  std::string m_message;
};
