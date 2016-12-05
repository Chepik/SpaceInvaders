#pragma once

#include <string>

struct Constants
{  
  constexpr static float kEps = 1e-5;
  constexpr static float PI = 3.1415927;
};

struct Globals
{
  static int Height;
  static int Width;
  static std::string SettingsFileName;
};
