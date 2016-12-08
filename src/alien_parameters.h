#pragma once

#include <sys/types.h>
#include "size_parameters.hpp"
#include "game_entity.hpp"

struct AlienParameters : SizeParameters
{
  uint m_rate = 0;
  size_t m_number = 0;
  int m_speed = 0;
  int m_health = 0;
  TSize m_size = std::make_pair(0, 0);
  size_t m_rowNumber = 0;
  uint m_frequency = 0;
};

