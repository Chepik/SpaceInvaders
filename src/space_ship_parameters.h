#pragma once

#include <sys/types.h>
#include "game_entity.hpp"
#include "size_parameters.hpp"

struct SpaceShipParameters : SizeParameters
{
  uint m_health = 0;
  uint m_rate = 0;
  uint m_speed = 0;
  TSize m_size = std::make_pair(0, 0);
};

