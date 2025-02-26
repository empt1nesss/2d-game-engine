#include "misc.hpp"

#if defined (_WIN32)
#include <corecrt_math_defines.h>
#elif defined(__linux)
#define _USE_MATH_DEFINES
#endif 
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
  if (v.x == 0.f && v.y == 0.f)
    return v;

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

float deg(float rad)
{
  return rad / M_PI * 180.f;
}

float rad(float deg)
{
  return deg * M_PI / 180.f;
}


Json::Value serialize_vector(const sf::Vector2f &v)
{
  return { v.x, v.y };
}

Json::Value serialize_color(const sf::Color &c)
{
  return {
    c.r,
    c.g,
    c.b,
    c.a
  };
}

