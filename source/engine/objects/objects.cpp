#include "objects.hpp"

#include <cmath>


std::vector<Engine::Object*> Engine::Object::m_objects;


Engine::Object::Object(const std::vector<sf::Vector2f> &vertices, sf::Vector2f pos) :
  EnableCollision(false),
  EnableMovement (false),
  EnableGravity  (false),
  EnableRotation (false),

  Speed       ({ 0.f, 0.f }),
  AngularSpeed(0.f),
  Mass        (0.f),

  DrawBody(false)
{
  m_objects.push_back(this);

  m_body.setPrimitiveType(sf::TriangleFan);

  m_body.resize(vertices.size());
  for (size_t i = 0; i < vertices.size(); ++i) {
    m_body[i].position = vertices[i];
    m_body[i].color    = sf::Color::Red;
    m_center += vertices[i];
  }

  m_center /= (float)vertices.size();

  SetPosition(pos);
}


void Engine::Object::Update(uint64_t dt)
{
  m_sprite.Update(dt);

  if (EnableGravity && EnableMovement) {
    Speed.y += (dt * 1e-9f * GravityAcceleration);
  } 
  if (EnableMovement) {
    sf::Vector2f s = Speed * (dt * 1e-9f);
    for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
      m_body[i].position += s;
    }
    m_center += s;
    m_sprite.move(s);
  }
  if (EnableRotation) {
    float as = AngularSpeed * (dt * 1e-9f);

    for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
      m_body[i].position -= m_center;

      m_body[i].position = {
        (float)(cos(as) * m_body[i].position.x +
        -sin(as) * m_body[i].position.y),

        (float)(sin(as) * m_body[i].position.x +
        cos(as) * m_body[i].position.y)
      };

      m_body[i].position += m_center;
    }

    m_sprite.rotate(as);
  }
  if (EnableCollision) {
    // to do
  }
}

void Engine::Object::Render(sf::RenderTarget &target)
{
  target.draw(m_sprite);
  if (DrawBody)
    target.draw(m_body);
}

void Engine::Object::SetPosition(sf::Vector2f pos)
{
  auto s = pos - m_center;
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;

  m_center = pos;

  m_sprite.setPosition(pos);
}

void Engine::Object::Move(sf::Vector2f s)
{
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;
  
  m_center += s;

  m_sprite.move(s);
}

void Engine::Object::SetTexture(TextureAtlas texture_atlas)
{
  m_sprite.SwitchAnimation(texture_atlas);
  m_sprite.setPosition(m_center);
}

void Engine::Object::SetSpriteScale(sf::Vector2f scale)
{
  m_sprite.setScale(scale);
}

void Engine::Object::SetSpriteOrigin(sf::Vector2f origin)
{
  m_sprite.setOrigin(origin);
  m_sprite.setPosition(m_center);
}

void Engine::Object::SetSpriteRotation(float angle)
{
  m_sprite.setRotation(angle);
}

void Engine::Object::SetBodyColor(sf::Color color)
{
  for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
    m_body[i].color = color;
  }
}




Engine::RectObject::RectObject(sf::Vector2f size, sf::Vector2f pos) :
  Object({
    { 0.f,    0.f    },
    { size.x, 0.f    },
    { size.x, size.y },
    { 0.f,    size.y }
  }, pos)
{}




Engine::CircleObject::CircleObject(float radius, sf::Vector2f pos, uint vertex_count) :
  Object(create_circle(radius, vertex_count), pos)
{}



std::vector<sf::Vector2f> Engine::CircleObject::create_circle(
  float radius, uint vertex_count
)
{
  std::vector<sf::Vector2f> res(vertex_count);

  for (uint v = 0; v < vertex_count; ++v) {
    float angle = 2.f * M_PI / vertex_count * v;
    res[v] = {
      (float)(sin(angle) * radius),
      (float)(cos(angle) * radius)
    };
  }

  return res;
}

