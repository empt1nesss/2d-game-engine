#include "phys-shape.hpp"

#include <cmath>

void Engine::Shape::Update(uint64_t dt)
{
  sf::Vector2f s  = m_v * (dt * 1e-6f);
  float angular_s = m_angular_v * (dt * 1e-9f);

  sf::Vector2f centre;

  for (size_t i = 0; i < m_vertecies.getVertexCount(); ++i) {
    m_vertecies[i].position += s;
    centre += m_vertecies[i].position;
  }

  centre /= (float)m_vertecies.getVertexCount();

  for (size_t i = 0; i < m_vertecies.getVertexCount(); ++i) {
    m_vertecies[i].position -= centre;

    m_vertecies[i].position = {
      (float)(cos(angular_s) * m_vertecies[i].position.x +
      -sin(angular_s) * m_vertecies[i].position.y),

      (float)(sin(angular_s) * m_vertecies[i].position.x +
      cos(angular_s) * m_vertecies[i].position.y)
    };

    m_vertecies[i].position += centre;
  }
}

void Engine::Shape::Render(sf::RenderTarget &target)
{
  target.draw(m_vertecies);
}
