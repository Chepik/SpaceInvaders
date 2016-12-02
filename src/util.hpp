#pragma once

#include "json/value.h"


class Util
{
public:
  Util() = delete;
  Util(Util const &) = delete;
  Util(Util const &&) = delete;
  Util & operator=(Util const &) = delete;
  Util & operator=(Util const &&) = delete;

  static void WriteJson(std::string const &  file_name,
                        Json::Value const & out);

  static Json::Value ReadJson(std::string const & file_name);
};
