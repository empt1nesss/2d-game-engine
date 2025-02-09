#include "misc.hpp"

#include <cmath>


float dot(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

float cross(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
  return v1.x * v2.y - v1.y * v2.x;
}

sf::Vector2f normalize(const sf::Vector2f &v)
{
  return v / std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f perpendicular(const sf::Vector2f &v)
{
  return { -v.y, v.x };
}

float length(const sf::Vector2f &v)
{
  return std::sqrt(v.x * v.x + v.y * v.y);
}

