#include "gtest/gtest.h"

#include <fstream>
#include <iostream>

#include "json/assertions.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/json.h"
#include "json/config.h"

#include "util.hpp"


TEST(jsoncpp_test, test_create_json_1)
{
  Json::Value settings;

  settings["Difficulty"] = "Easy";
  settings["Speed"] = "Speed 1";

  std::cout << settings << std::endl;

  EXPECT_EQ(settings["Speed"], "Speed 1");
}

TEST(jsoncpp_test, test_create_json_2)
{
  Json::Value settings;

  std::stringstream ss;

  ss << "\{\"Difficulty\" : \"Easy\",\"Speed\" : \"Speed 1\"}";

  ss >> settings;

  std::cout << settings << std::endl;
}

TEST(jsoncpp_test, test_write_json_1)
{
  Json::Value settings;

  settings["Difficulty"] = "Easy";
  settings["Speed"] = "Speed 1";

  std::string file_name = "test_1.json";

  Util::WriteJson(file_name, settings);
}

TEST(jsoncpp_test, test_read_json_1)
{
  std::string file_name = "test_1.json";

  Json::Value settings = Util::ReadJson(file_name);

  EXPECT_EQ(settings["Difficulty"], "Easy");
  EXPECT_EQ(settings["Speed"], "Speed 1");
}
