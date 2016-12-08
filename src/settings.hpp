#pragma once

#include "singleton.h"
#include "game_entity.hpp"
#include "bullet_parameters.hpp"
#include "explosion_parameters.hpp"
#include "alien_parameters.h"
#include "star_parameters.h"
#include "space_ship_parameters.h"
#include "obstacle_parameters.h"
#include "main_parameters.hpp"

class Settings : public Singleton<Settings>
{
public:
  ///
  /// Load parameters which don't depend on level number.
  ///
  /// Exception: ReadSettingsException.
  ///
  void LoadMainSettings();

  ///
  /// Load parameters which depend on level number.
  ///
  /// Exception: ReadSettingsException.
  ///
  void LoadLevelSettings(const std::string & level);

  /// Main parameters.
  MainParameters m_mainParameters;

  /// Star parameters.
  StarParameters m_starParameters;

  /// Explosion parameters.
  ExplosionParameters m_explosionParameters;

  /// Alien parameters.
  AlienParameters m_alienParameters;

  /// Bullet parameters.
  BulletParameters m_bulletParameters;

  /// Space Ship parameters.
  SpaceShipParameters m_spaceShipParameters;

  /// Obstacle parameters.
  ObstacleParameters m_obstacleParameters;

private:
  /// Otherwise it won't be accessible in parent class Singleton<Settings>.
  friend class Singleton<Settings>;

  Settings() = default;
};
