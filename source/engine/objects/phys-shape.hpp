#ifndef GAME_PHYS_SHAPE_HPP
#define GAME_PHYS_SHAPE_HPP


#include "engine/engine.hpp"


class Engine::Shape
{
public:

  Shape(const sf::VertexArray &verticies) :
    m_vertecies(verticies),
    m_v(),
    m_angular_v(0.f),
    m_weight(1.f)
  {}

  ~Shape() = default;


  void Update(uint64_t dt);

  void Render(sf::RenderTarget &target);

private:

  sf::VertexArray m_vertecies;

  sf::Vector2f m_v;
  float        m_angular_v;
  float        m_weight;


  friend class Engine;

};


#endif // !GAME_PHYS_SHAPE_HPP
