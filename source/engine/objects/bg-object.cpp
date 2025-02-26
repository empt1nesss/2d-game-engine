#include "bg-object.hpp"

Engine::BgObject::BgObject(
  TextureAtlas     texture_atlas,
  ResourceManager &rm,
  float            depth,
  sf::Vector2f     pos
) :
  Object({ { 0.f, 0.f } }, pos),
  m_depth(depth)
{
  Object::GetSprite().SwitchAnimation(texture_atlas, rm);

  EnableMovement(true);
  EnableRotation(true);
}


void Engine::BgObject::Update(uint64_t dt)
{
  Object::Update(dt);
}

void Engine::BgObject::Render(sf::RenderTarget &target)
{
  Object::Render(target);
}

void Engine::BgObject::SetDepth(float depth)
{
  if (depth > 1.f)
    m_depth = 1.f;
  else if (depth < 0.f)
    m_depth = 0.f;
  else
    m_depth = depth;
}

Json::StructType Engine::BgObject::Serialize() const
{
  return {
    { "sprite", Object::GetSprite().Serialize() },
    { "depth",  m_depth }
  };
}



