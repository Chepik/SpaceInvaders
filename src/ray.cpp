#include "ray.hpp"

#include <math.h>
#include <array>
#include <algorithm>

#include "constants.hpp"
#include "box2d.hpp"


void Ray::swap(Ray & rhs)
{
  std::swap(m_origin, rhs.m_origin);
  std::swap(m_direction, rhs.m_direction);
}

Ray::Ray(Ray && rhs)
{
  swap(rhs);

  std::cout << "Move copy constructor" <<std::endl;
}

Ray & Ray::operator=(Ray && rhs)
{
  swap(rhs);

  std::cout << "Move operator=" <<std::endl;

  return *this;
}

bool Ray::operator ==(Ray const & rhs) const
{
  if (this == &rhs) { return true; }

  if (this->m_direction != rhs.direction()) { return false; }
  if (this->m_origin != rhs.origin()) { return false; }

  return true;
}

bool Ray::operator !=(Ray const & rhs) const
{
  return !(this->operator ==(rhs));
}

Ray & Ray::operator=(const Ray & rhs)
{
  Ray tmp(rhs);
  swap(tmp);
  return *this;
}

bool Ray::checkIntersection(
    Ray const & ray,
    Box2D const & box)
{
  // Check if a point inside a box.
  if (Box2D::checkInside(box, ray.origin()))
  {
    return true;
  }

  bool is_special = false;

  // Recalculate box points.
  // Use the point as an origin of a coordinate system for the box.
  float x_1_n = box.point1().x() - ray.origin().x();
  float x_2_n = box.point2().x() - ray.origin().x();

  float y_1_n = box.point1().y() - ray.origin().y();
  float y_2_n = box.point2().y() - ray.origin().y();

  float x_3_n = x_1_n;
  float y_3_n = y_2_n;

  float x_4_n = x_2_n;
  float y_4_n = y_1_n;

  float x_ray = ray.direction().x();
  float y_ray = ray.direction().y();

  // It handels special case when rectangle intersepts x-axis
  // to the right from an origin point of a ray.
  if (x_ray < x_1_n && x_ray < x_2_n)
  {
    is_special = true;
  }

  // Check if the x coordinate is zero.
  x_ray = checkZeroDenominator(x_ray);
  x_1_n = checkZeroDenominator(x_1_n);
  x_2_n = checkZeroDenominator(x_2_n);
  x_3_n = checkZeroDenominator(x_3_n);
  x_4_n = checkZeroDenominator(x_4_n);

  // Calculate an anlge for the current point
  // and convert the angle to degrees.
  float angle_ray = convertRadianToDegrees(
        std::atan(y_ray / x_ray));
  float angle_1_n = convertRadianToDegrees(
        std::atan(y_1_n / x_1_n));
  float angle_2_n = convertRadianToDegrees(
        std::atan(y_2_n / x_2_n));
  float angle_3_n = convertRadianToDegrees(
        std::atan(y_3_n / x_3_n));
  float angle_4_n = convertRadianToDegrees(
        std::atan(y_4_n / x_4_n));

  // Correct a value of the angle if needed.
  angle_ray = recalculateAngle(x_ray, y_ray, angle_ray);
  angle_1_n = recalculateAngle(x_1_n, y_1_n, angle_1_n);
  angle_2_n = recalculateAngle(x_2_n, y_2_n, angle_2_n);
  angle_3_n = recalculateAngle(x_3_n, y_3_n, angle_3_n);
  angle_4_n = recalculateAngle(x_4_n, y_4_n, angle_4_n);

  // Combine all angles for the box points.
  std::array<float, 4> angles = {
    angle_1_n, angle_2_n, angle_3_n, angle_4_n
  };

  if (is_special)
  {
    // It finds to angles.
    // Maximum angle in the first quater
    // and the minimun angle in the fouth quater.
    float angle_max_first_quater = 0.0f;
    float angle_min_fourth_quater = 360.0f;

    for (auto const & item : angles)
    {
      if (item < 90.0f)
      {
        if (angle_max_first_quater < item)
        {
          angle_max_first_quater = item;
        }
      }
      else if (item > 270.0f)
      {
        if (angle_min_fourth_quater > item)
        {
          angle_min_fourth_quater = item;
        }
      }
    }

    // Compare the ray angle with
    // the max angle in the first quater
    // and the min angle in the fouth quater.
    if (angle_ray <= angle_max_first_quater &&
        angle_ray >= angle_min_fourth_quater) {
      return true;
    }
  }
  else
  {
    // Find a min and max angles for the box points.
    auto result = std::minmax_element(
          std::begin(angles), std::end(angles));

    // Compare the ray angle with the min
    // and max angles for the box points.
    if (*result.first <= angle_ray &&
        angle_ray <= *result.second) {
      return true;
    }
  }

  return false;
}

float Ray::convertRadianToDegrees(float const & angle)
{
  return angle / Constants::PI * 180;
}

float Ray::recalculateAngle(float const & x,
                            float const & y,
                            float const & angle)
{
  float angle_n = 0.0f;

  // The second quarter.
  if (y > (0.0f - Constants::kEps) &&
      x < (0.0f + Constants::kEps))
  {
    angle_n = 180.0f - abs(angle);
  }

  // The third quarter.
  if (y < 0.0f &&
      x < (0.0f + Constants::kEps))
  {
    angle_n = 180.0f + abs(angle);
  }

  // The fouth quarter.
  if (y < 0.0f &&
      x > 0.0f)
  {
    angle_n = 360.0f - abs(angle);
  }

  return angle_n;
}

float Ray::checkZeroDenominator(float const & value)
{
  float tmp = value;

  if (abs(tmp) < Constants::kEps)
  {
    tmp = Constants::kEps;
  }

  return tmp;
}

std::ostream & operator <<(std::ostream & os,
                           Ray const & rhs)
{
  os << "origin: " << rhs.origin() << "; "
     << "direction: " << rhs.direction() << std::endl;
  return os;
}
