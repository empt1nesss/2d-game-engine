#include "misc.hpp"

#if defined (_WIN32)
#include <corecrt_math_defines.h>
#elif defined(__linux)
#define _USE_MATH_DEFINES
#endif 
#include <cmath>


Json::Value TextureAtlas::Serialize() const
{
  return {
    Json::Property
    { "texture_alias", texture_alias },
    { "frame_width", frame_width },
    { "frame_height", frame_height },
    { "rows", rows },
    { "cols", cols },
    { "offset_x", offset_x },
    { "offset_y", offset_y }
  };
}

TextureAtlas& TextureAtlas::Deserialize(const Json::Value &val)
{
  texture_alias = val["texture_alias"].GetString();
  frame_width = val["frame_width"].GetInt();
  frame_height = val["frame_height"].GetInt();
  rows = val["rows"].GetInt();
  cols = val["cols"].GetInt();
  offset_x = val["offset_x"].GetInt();
  offset_y = val["offset_y"].GetInt();

  return *this;
}


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

sf::Vector2f deserialize_vector(const Json::Value &val)
{
  return { (float)val[0].GetFloat(), (float)val[1].GetFloat() };
}

sf::Color deserialize_color (const Json::Value &val)
{
  sf::Color out;

  return {
    (sf::Uint8)val[0].GetInt(),
    (sf::Uint8)val[1].GetInt(),
    (sf::Uint8)val[2].GetInt(),
    (sf::Uint8)val[3].GetInt()
  };
}

