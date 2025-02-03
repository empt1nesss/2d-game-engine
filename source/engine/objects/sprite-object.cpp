#include "objects.hpp"


Engine::SpriteObject::SpriteObject()
{

}


void Engine::SpriteObject::Update(uint64_t dt)
{
  if (EnableGravity && EnableMovement) {
    Speed.y += (dt * 1e-9f * GravityAcceleration);
  }
  else if (EnableMovement) {
    sf::Vector2f s = Speed * (dt * 1e-9f);
    m_sf_sprite.move(s);
  }

  if (EnableRotation) {
    m_sf_sprite.rotate(AngularSpeed * (dt * 1e-9f));
  }

  if (EnableCollision) {
    // to do
  }
}

void Engine::SpriteObject::Render(sf::RenderTarget &target)
{
  target.draw(m_sf_sprite);
}

void Engine::SpriteObject::SetPosition(sf::Vector2f pos)
{
  m_sf_sprite.setPosition(pos);
}

void Engine::SpriteObject::SetTexture(sf::Texture &texture)
{
  m_sf_sprite.setTexture(texture);

  auto pos = m_sf_sprite.getPosition();
  m_sf_sprite.setOrigin(m_sf_sprite.getLocalBounds().getSize() / 2.f);
  m_sf_sprite.setPosition(pos);
}

