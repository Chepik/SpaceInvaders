#include "point2d.hpp"

void Point2D::swap(Point2D &rhs)
{
  std::swap(m_x, rhs.m_x);
  std::swap(m_y, rhs.m_y);
}
