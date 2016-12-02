#include "gtest/gtest.h"
#include "alien.hpp"
#include "space_ship.hpp"
#include "obstacle.hpp"
#include "bullet_manager.hpp"
#include "space.hpp"
#include "except.hpp"
#include "factory.hpp"

TEST(factory_test, test_construction)
{
  Factory factory;
  auto bullet = factory.Create<Bullet>(15,Point2D(1.0f, 1.0f));
  EXPECT_EQ(bullet->GetName(),"Bullet");
  EXPECT_EQ(bullet->GetDamage(),15);
  EXPECT_EQ(bullet->GetPosition(),Point2D(1.0f, 1.0f));

  auto bulletManager = factory.Create<BulletManager>();
  EXPECT_EQ(bulletManager->GetBullets().empty(),true);

  auto alien1 = factory.Create<Alien>(15,Point2D(1.0f, 1.0f), 1115, 25);
  EXPECT_EQ(alien1->GetName(),"Alien");
  EXPECT_EQ(alien1->GetSpeed(),15);
  EXPECT_EQ(alien1->GetPosition(),Point2D(1.0f, 1.0f));
  EXPECT_EQ(alien1->GetRate(),1115);
  EXPECT_EQ(alien1->GetHealth(),25);

  auto spaceship1 = factory.Create<SpaceShip>(Point2D(1.0f, 1.0f), 1115, 25);
  EXPECT_EQ(spaceship1->GetName(),"SpaceShip");
  EXPECT_EQ(spaceship1->GetPosition(),Point2D(1.0f, 1.0f));
  EXPECT_EQ(spaceship1->GetRate(),1115);
  EXPECT_EQ(spaceship1->GetHealth(),25);
}
