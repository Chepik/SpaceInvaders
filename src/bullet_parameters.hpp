#pragma once

#include "game_entity.hpp"
#include "size_parameters.hpp"

struct BulletParameters :  SizeParameters
{
  uint m_damage = 0;
  TSize m_size = std::make_pair(0, 0);
};
