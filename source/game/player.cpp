#include "player.hpp"


Game::Player::Player(sf::Texture *texture) :
  m_sprite(*texture),
  m_ms(500.f)
{
  // m_sprite.setScale(sf::Vector2f{ 2.f, 2.f });
  m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

Game::Player::~Player()
{

}


void Game::Player::Update(uint64_t dt, ButtonState keyboard_state[])
{
  float ms   = m_ms * (dt * 1e-9);
  float d_ms = sqrt(pow(m_ms, 2) - pow(m_ms / 2.f, 2)) * (dt * 1e-9);

  if (
    (int)keyboard_state[sf::Keyboard::A] > 0 &&
    (int)keyboard_state[sf::Keyboard::W] > 0
  ) {
    m_sprite.move({ -d_ms, -d_ms });
  }
  else if (
    (int)keyboard_state[sf::Keyboard::D] > 0 &&
    (int)keyboard_state[sf::Keyboard::W] > 0
  ) {
    m_sprite.move({ d_ms, -d_ms });
  }
  else if (
    (int)keyboard_state[sf::Keyboard::A] > 0 &&
    (int)keyboard_state[sf::Keyboard::S] > 0
  ) {
    m_sprite.move({ -d_ms, d_ms });
  }
  else if (
    (int)keyboard_state[sf::Keyboard::D] > 0 &&
    (int)keyboard_state[sf::Keyboard::S] > 0
  ) {
    m_sprite.move({ d_ms, d_ms });
  }
  else if ((int)keyboard_state[sf::Keyboard::W] > 0)
    m_sprite.move({ 0.f, -ms });
  else if ((int)keyboard_state[sf::Keyboard::S] > 0)
    m_sprite.move({ 0.f, ms });
  else if ((int)keyboard_state[sf::Keyboard::A] > 0)
    m_sprite.move({ -ms, 0.f });
  else if ((int)keyboard_state[sf::Keyboard::D] > 0)
    m_sprite.move({ ms, 0.f });
}

void Game::Player::Render(sf::RenderTarget &target)
{
  target.draw(m_sprite);
}
