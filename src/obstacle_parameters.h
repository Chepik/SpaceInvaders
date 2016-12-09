#pragma once

#include <sys/types.h>
#include "game_entity.hpp"
#include "size_parameters.hpp"

struct ObstacleParameters : SizeParameters
{
  size_t m_number = 0;
  int m_health = 0;
  TSize m_size = std::make_pair(0, 0);
  size_t m_score = 0;
};

