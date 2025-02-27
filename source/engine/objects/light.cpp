#include "light.hpp"

#if defined (_WIN32)
#include <corecrt_math_defines.h>
#elif defined(__linux)
#define _USE_MATH_DEFINES
#endif 
#include <cmath>


Engine::Light::Light(float radius, sf::Color color, sf::Vector2f pos, unsigned vertex_count) : 
  Engine::Object(std::move(Object::CreateCircleObj(radius, pos, vertex_count))),
  m_radius          (radius),
  m_color           (color),
  m_pos             (pos),
  m_vertex_count    (vertex_count),
  m_brightness_level(m_color.a),
  m_angle           (2 * M_PI),
  m_rotation        (0.0f)
{
  add_center();
  SetColor(m_color);

  ZIndex = 100;
  DrawBody = true;
  EnableMovement(true);
  EnableRotation(true);
}

void Engine::Light::SetColor(sf::Color color)
{
  m_color = color;
  Object::SetBodyColor(color);
  create_gradient();
}

void Engine::Light::SetRadius(float radius)
{
  if (m_radius == radius)
    return;
  m_radius = radius;

  *(Engine::Object*)this = Object::CreateCircleObj(m_radius, m_pos, m_vertex_count);
  
  SetColor(m_color);
}

void Engine::Light::SetRadius(float radius, unsigned vertex_count)
{
  if (m_radius == radius)
    return;

  m_radius = radius;
  m_vertex_count = vertex_count;

  *(Engine::Object*)this = Object::CreateCircleObj(m_radius, m_pos, m_vertex_count);

  add_center();
  SetColor(m_color);
}

void Engine::Light::SetAngle(float angle)
{
  if (m_angle == angle)
    return;

  m_angle = angle;

  float y_dot = m_pos.y + m_radius * cos(angle/2);

  for(int it = 0; it < m_vertex_count; ++it)
  {
    if (m_body[it].position.y < y_dot)
    {
      m_body[it].position = m_pos;
    }
    else
      m_body[it].position = m_body[it].position;
  }
  create_gradient();
}

void Engine::Light::SetPosition (sf::Vector2f pos)
{
  if (m_pos == pos)
    return;
  
  m_pos = pos;

  Object::SetPosition(m_pos);
  create_gradient();
}

void Engine::Light::SetRotation (float angle)
{
  if (m_angle >= 2 * M_PI)
    return;

  float delta = angle - m_rotation;

  Object::Rotate(delta, m_pos);
  
  m_rotation = angle;
}


void Engine::Light::SetBrightnessLevel(uint8_t brightness_level)
{
  if (m_brightness_level == brightness_level)
    return;

  m_brightness_level = brightness_level;
  m_color.a = m_brightness_level;
  create_gradient();
}


void Engine::Light::create_gradient()
{
  for(int it = 0; it < m_vertex_count; ++it)
  {
    if (m_body[it].position == m_pos)
      m_body[it].color = m_color;
    else
      m_body[it].color = {m_color.r, m_color.g, m_color.b, 0};
  }
}

void Engine::Light::add_center()
{
  m_body.resize(m_vertex_count + 2);
  m_body[m_vertex_count].position = m_body[0].position;
  m_body[0].position = m_pos;
  m_body[m_vertex_count + 1].position = m_body[1].position;
  
  m_body[m_vertex_count].color = m_color;
  m_body[m_vertex_count + 1].color = m_color;
  m_vertex_count += 2;
}

