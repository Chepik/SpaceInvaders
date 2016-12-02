#include "util.hpp"

#include <fstream>
#include <iostream>

#include "json/assertions.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/json.h"
#include "json/config.h"


void Util::WriteJson(std::string const &  file_name,
                     Json::Value const & out)
{
  std::ofstream ofs(file_name);

  if (ofs.is_open())
  {
    Json::StyledWriter styledWriter;

    ofs << styledWriter.write(out);

    ofs.close();
  }
}

Json::Value Util::ReadJson(std::string const & file_name)
{
  std::ifstream ifs(file_name, std::ifstream::binary);

  Json::Value root;

  if (ifs.is_open())
  {
    ifs >> root;

    ifs.close();
  }

  return root;
}
