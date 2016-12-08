#include "settings.hpp"

#include "json/assertions.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/json.h"

#include "util.hpp"
#include "except.hpp"

void Settings::LoadMainSettings()
{
  Json::Value settings;

  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);

    // MainParameters.
    m_mainParameters.m_difficulty = settings["Difficulty"].asUInt();
    m_mainParameters.m_speed = settings["Speed"].asUInt();
    m_mainParameters.m_levelsNumber = settings["LevelsNumber"].asUInt();

    // StarParameters
    m_starParameters.m_number = settings["StarNumber"].asUInt();
    m_starParameters.m_size = std::make_pair(
        settings["StarWidth"].asInt(), settings["StarHeigth"].asInt());

    // Explosion parameters.
    m_explosionParameters.m_lifetime = settings["ExplosionLifeTime"].asUInt();

    m_explosionParameters.m_lifetimeBig = settings["ExplosionLifeTimeBig"].asUInt();

    m_explosionParameters.m_size = std::make_pair(
        settings["ExplosionWidth"].asInt(),
        settings["ExplosionHeight"].asInt());

    m_explosionParameters.m_sizeBig = std::make_pair(
        settings["ExplosionWidthBig"].asInt(),
        settings["ExplosionHeightBig"].asInt());
  }
  catch(ReadFileException const & ex)
  {
    throw ReadSettingsException(Globals::SettingsFileName);
  }
}

void Settings::LoadLevelSettings(std::string const & level)
{
  Json::Value settings;

  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);

    /// Alien parameters.
    m_alienParameters.m_number = settings["Level"][level]["AliensNumber"].asUInt();
    m_alienParameters.m_speed = settings["Level"][level]["AlienSpeed"].asInt();
    m_alienParameters.m_rate = settings["Level"][level]["AlienRate"].asUInt();
    m_alienParameters.m_health = settings["Level"][level]["AlienHealth"].asInt();

    m_alienParameters.m_size = std::make_pair(
        settings["Level"][level]["AlienWidth"].asInt(),
        settings["Level"][level]["AlienHeigth"].asInt());

    m_alienParameters.m_rowNumber = settings["Level"][level]["AlienRowNumber"].asUInt();
    m_alienParameters.m_frequency = settings["Level"][level]["AlienFrequency"].asUInt();

    /// Bullet parameters.
    m_bulletParameters.m_damage = settings["Level"][level]["BulletDamage"].asUInt();

    m_bulletParameters.m_size = std::make_pair(
        settings["Level"][level]["BulletWidth"].asInt(),
        settings["Level"][level]["BulletHeight"].asInt());

    /// Space Ship parameters.
    m_spaceShipParameters.m_health = settings["Level"][level]["SpaceShipHealth"].asInt();
    m_spaceShipParameters.m_speed = settings["Level"][level]["SpaceShipSpeed"].asUInt();

    m_spaceShipParameters.m_size = std::make_pair(
        settings["Level"][level]["SpaceShipWidth"].asInt(),
        settings["Level"][level]["SpaceShipHeigth"].asInt());

    m_spaceShipParameters.m_rate = settings["Level"][level]["SpaceShipRate"].asUInt();

    /// Obstacle parameters.
    m_obstacleParameters.m_number = settings["Level"][level]["ObstacleNumber"].asUInt();
    m_obstacleParameters.m_health = settings["Level"][level]["ObstacleHealth"].asInt();

    m_obstacleParameters.m_size = std::make_pair(
        settings["Level"][level]["ObstacleWidth"].asInt(),
        settings["Level"][level]["ObstacleHeigth"].asInt());

  }
  catch(ReadFileException const & ex)
  {
    throw ReadSettingsException(Globals::SettingsFileName);
  }
}