#include "gtest/gtest.h"
#include "ray.hpp"

TEST(ray_test, test_construction)
{
  // Default constructor.
  Ray r1;
  EXPECT_FLOAT_EQ(r1.origin().x(), 0.0f);
  EXPECT_FLOAT_EQ(r1.origin().y(), 0.0f);
  EXPECT_FLOAT_EQ(r1.direction().x(), 0.0f);
  EXPECT_FLOAT_EQ(r1.direction().y(), 0.0f);

  // Constructor with parameters.
  Ray r2(Point2D(1.0f, 1.0f),
         Point2D(0.0f, 1.0f));

  EXPECT_FLOAT_EQ(r2.origin().x(), 1.0f);
  EXPECT_FLOAT_EQ(r2.origin().y(), 1.0f);
  EXPECT_FLOAT_EQ(r2.direction().x(), 0.0f);
  EXPECT_FLOAT_EQ(r2.direction().y(), 1.0f);

  // Copy constructor.
  Ray r3 = r2;

  EXPECT_EQ(r3 == r2, true);
}

TEST(ray_test, test_assignment)
{
  Ray r1;

  r1 = Ray(Point2D(1.0f, 1.0f),
           Point2D(0.0f, 1.0f));

  EXPECT_FLOAT_EQ(r1.origin().x(), 1.0f);
  EXPECT_FLOAT_EQ(r1.origin().y(), 1.0f);
  EXPECT_FLOAT_EQ(r1.direction().x(), 0.0f);
  EXPECT_FLOAT_EQ(r1.direction().y(), 1.0f);
}

TEST(ray_test, test_equality)
{
  Ray r1(Point2D(1.0f, 1.0f),
         Point2D(0.0f, 1.0f));

  Ray r2(Point2D(1.0f, 1.0f),
         Point2D(0.0f, 1.0f));

  Ray r3(Point2D(1.0f, 1.0f),
         Point2D(1.0f, 1.0f));

  EXPECT_EQ(r1, r2);
  EXPECT_NE(r1, r3);
}

TEST(ray_test, test_intersection_outside)
{
  // Send a ray to the right.
  Ray r1(Point2D(0.0f, 0.0f),
         Point2D(1.0f, 0.0f));

  Box2D box1 = { Point2D(1.0f, -1.0f),
                 Point2D(4.0f, 1.0f) };

  EXPECT_EQ(Ray::checkIntersection(r1, box1), true);

  // Send a ray to the left.
  Ray r2(Point2D(0.0f, 0.0f),
         Point2D(-1.0f, 0.0f));

  Box2D box2 = { Point2D(-1.0f, -1.0f),
                 Point2D(-4.0f, 1.0f) };

  EXPECT_EQ(Ray::checkIntersection(r2, box2), true);

  // Send a ray to the upwards.
  Ray r3(Point2D(0.0f, 0.0f),
         Point2D(0.0f, 1.0f));

  Box2D box3 = { Point2D(-1.0f, 1.0f),
                 Point2D(1.0f, 4.0f) };

  EXPECT_EQ(Ray::checkIntersection(r3, box3), true);

  // Send a ray to the bottom.
  Ray r4(Point2D(0.0f, 0.0f),
         Point2D(0.0f, -1.0f));

  Box2D box4 = { Point2D(-1.0f, -1.0f),
                 Point2D(1.0f, -4.0f) };

  EXPECT_EQ(Ray::checkIntersection(r4, box4), true);
}

TEST(ray_test, test_radian_to_degree)
{
  EXPECT_FLOAT_EQ(Ray::convertRadianToDegrees(Constants::PI), 180);
  EXPECT_FLOAT_EQ(Ray::convertRadianToDegrees(0.5 * Constants::PI), 90);
}

TEST(ray_test, test_intersection_inside)
{
  // Ray origin is inside a box.
  Ray r1(Point2D(0.0f, 0.0f),
         Point2D(1.0f, 0.0f));

  Box2D box1 = { Point2D(-1.0f, -1.0f),
                 Point2D(1.0f, 1.0f) };

  EXPECT_EQ(Ray::checkIntersection(r1, box1), true);
}

// The test contains an error
// Ray::checkIntersection(r1, box1).
TEST(ray_test, test_without_intersection_outside)
{
  // Send a ray to the right.
  Ray r1(Point2D(0.0f, 0.0f),
         Point2D(-1.0f, 0.0f));

  Box2D box1 = { Point2D(1.0f, -1.0f),
                 Point2D(4.0f, 1.0f) };

  EXPECT_EQ(Ray::checkIntersection(r1, box1), false);

  // Send a ray to the left.
  Ray r2(Point2D(0.0f, 0.0f),
         Point2D(1.0f, 0.0f));

  Box2D box2 = { Point2D(-1.0f, -1.0f),
                 Point2D(-4.0f, 1.0f) };

  EXPECT_EQ(Ray::checkIntersection(r2, box2), false);

  // Send a ray to the upwards.
  Ray r3(Point2D(0.0f, 0.0f),
         Point2D(0.0f, -1.0f));

  Box2D box3 = { Point2D(-1.0f, 1.0f),
                 Point2D(1.0f, 4.0f) };

  EXPECT_EQ(Ray::checkIntersection(r3, box3), false);

  // Send a ray to the bottom.
  Ray r4(Point2D(0.0f, 0.0f),
         Point2D(0.0f, 1.0f));

  Box2D box4 = { Point2D(-1.0f, -1.0f),
                 Point2D(1.0f, -4.0f) };

  EXPECT_EQ(Ray::checkIntersection(r4, box4), false);
}

TEST(ray_test, move)
{
  Ray r1(Point2D(0.0f, 0.0f),
         Point2D(0.0f, 1.0f));

  Ray r2 = std::move(r1);

  Ray r3;

  r3 = std::move(r1);
}

