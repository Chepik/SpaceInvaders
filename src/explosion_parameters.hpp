#pragma once

#include "game_entity.hpp"
#include "size_parameters.hpp"

struct ExplosionParameters : SizeParameters
{
  uint m_lifetime = 0;
  uint m_lifetimeBig = 0;
  TSize m_size = std::make_pair(0, 0);;
  TSize m_sizeBig = std::make_pair(0, 0);
};
