#pragma once

#include <list>
#include "GameEntity.h"

class Space
{
public:
  Space() = default;
  Space(uint const & width,uint const & height)
    : m_width(width),
      m_height(height)
  {}
  virtual ~Space();
  uint GetWidth() const;
  uint GetHeight() const;
  void SetWidth(uint const & width);
  void SetHeight(uint const & height);
  std::list<pGameEntity> GetGameEntity() const;
  void AddGameEntity(pGameEntity const & gameEntity);
private:
  uint m_width = 0;
  uint m_height = 0;
  std::list<pGameEntity> m_game_entity_list;
};
