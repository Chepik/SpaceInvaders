#pragma once

#include "point2d.hpp"
#include "box2d.hpp"

class Ray
{
public:
  Ray() = default;

  Ray(Point2D const & origin,
      Point2D const & direction)
    : m_origin(origin),
      m_direction(direction)
  {}

  void swap(Ray & rhs);

  Ray(Ray const & rhs)
    : m_origin(rhs.m_origin),
      m_direction(rhs.m_direction)
  {}

  Ray(Ray && rhs);

  // It uses "copy and swap" idiom.
  Ray & operator=(Ray const & rhs);

  Ray & operator=(Ray && rhs);

  bool operator == (Ray const & rhs) const;

  bool operator != (Ray const & rhs) const;

  ///
  /// Check if a ray intersect a box.
  ///
  static bool checkIntersection(Ray const & ray,
                                Box2D const & box);

  ///
  /// Convert radian to degrees.
  ///
  static float convertRadianToDegrees(
      float const & angle);

  ///
  /// Convert an angle from 0-90 to 0-360 degrees.
  ///
  static float recalculateAngle(
      float const & x,
      float const & y,
      float const & angle);

  ///
  /// Check zero denominator.
  ///
  /// If it equalts to zero then simply return a small number.
  ///
  static float checkZeroDenominator(float const & value);

  Point2D & origin() { return m_origin; }
  Point2D & direction() { return m_direction; }

  Point2D const & origin() const { return m_origin; }
  Point2D const & direction() const { return m_direction; }

private:
  Point2D m_origin;
  Point2D m_direction;
};

// Output to the console.
std::ostream & operator << (std::ostream & os,
                            Ray const & rhs);
