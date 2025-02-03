#include "player.hpp"

#include <cmath>


Engine::Player::Player(TextureAtlas idle, TextureAtlas run) :
  Engine::RectObject({ 60.f, 80.f }),
  m_ms(500.f),
  m_is_running(false),
  m_idle_anim(idle),
  m_run_anim(run)
{
  SetTexture(idle);
  SetSpriteScale(sf::Vector2f{ 4.f, 4.f });
  SetSpriteOrigin({ 60.f, 60.f });

  EnableGravity   = false;
  EnableMovement  = true;
  EnableCollision = true;

  DrawBody = true;
}

Engine::Player::~Player()
{

}


void Engine::Player::Update(uint64_t dt, const UserInput &user_input)
{
  float ms   = m_ms * (dt * 1e-9);
  float d_ms = sqrt(pow(m_ms, 2) - pow(m_ms / 2.f, 2)) * (dt * 1e-9);

  sf::Vector2f s = { 0.f, 0.f };

  bool running = true;

  if (
    user_input.GetKeyState(sf::Keyboard::A) > 0 &&
    user_input.GetKeyState(sf::Keyboard::W) > 0
  ) {
    s = { -d_ms, -d_ms };
  }
  else if (
    user_input.GetKeyState(sf::Keyboard::D) > 0 &&
    user_input.GetKeyState(sf::Keyboard::W) > 0
  ) {
    s = { d_ms, -d_ms };
  }
  else if (
    user_input.GetKeyState(sf::Keyboard::A) > 0 &&
    user_input.GetKeyState(sf::Keyboard::S) > 0
  ) {
    s = { -d_ms, d_ms };
  }
  else if (
    user_input.GetKeyState(sf::Keyboard::D) > 0 &&
    user_input.GetKeyState(sf::Keyboard::S) > 0
  ) {
    s = { d_ms, d_ms };
  }
  else if (user_input.GetKeyState(sf::Keyboard::W) > 0)
    s = { 0.f, -ms };
  else if (user_input.GetKeyState(sf::Keyboard::S) > 0)
    s = { 0.f, ms };
  else if (user_input.GetKeyState(sf::Keyboard::A) > 0)
    s = { -ms, 0.f };
  else if (user_input.GetKeyState(sf::Keyboard::D) > 0)
    s = { ms, 0.f };
  else
    running = false;

  Move(s);

  if (m_is_running != running) {
    m_is_running = running;
    if (running)
      SetTexture(m_run_anim);
    else
      SetTexture(m_idle_anim);
  }

  RectObject::Update(dt);
}

