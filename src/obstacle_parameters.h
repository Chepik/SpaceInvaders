#pragma once

#include <sys/types.h>
#include "game_entity.hpp"
#include "size_parameters.hpp"

struct ObstacleParameters : SizeParameters
{
  size_t m_number = 0;
  uint m_health = 0;
  TSize m_size = std::make_pair(0, 0);
};

