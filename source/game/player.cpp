#include "player.hpp"


Game::Player::Player(Animation idle, Animation run) :
  m_sprite(idle),
  m_ms(.5f),
  m_is_running(false),
  m_idle_anim(idle),
  m_run_anim(run)
{
  m_sprite.setOrigin(60, 60);
  m_sprite.setScale(sf::Vector2f{ 4.f, 4.f });
}

Game::Player::~Player()
{

}


void Game::Player::Update(uint64_t dt, ButtonState keyboard_state[])
{
  m_sprite.Update(dt);

  float ms   = m_ms * (dt * 1e-6);
  float d_ms = sqrt(pow(m_ms, 2) - pow(m_ms / 2.f, 2)) * (dt * 1e-6);

  bool running = true;

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
  else
    running = false;

  if (m_is_running != running) {
    m_is_running = running;
    if (running)
      m_sprite.SwitchAnimation(m_run_anim);
    else
      m_sprite.SwitchAnimation(m_idle_anim);
  }
}

void Game::Player::Render(sf::RenderTarget &target)
{
  target.draw(m_sprite);
}
