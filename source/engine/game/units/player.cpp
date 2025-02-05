#include "player.hpp"

#include <cmath>


Engine::Game::Player::Player(TextureAtlas idle, TextureAtlas run) :
  Engine::RectObject({ 60.f, 80.f }),
  m_ms(500.f),
  m_is_running(false),
  m_idle_anim(idle),
  m_run_anim(run)
{
  SetTexture(idle);
  SetSpriteScale(sf::Vector2f{ 4.f, 4.f });
  SetSpriteOrigin({ 60.f, 60.f });
  // SetRestitution(1.f);

  EnableMovement(true);
  EnableGravity(true);
  EnableCollision(true);

  DrawBody = true;

  // SetSpeed({ 250.f, -100.f });
}

Engine::Game::Player::~Player()
{

}


void Engine::Game::Player::Update(uint64_t dt, const UserInput &user_input)
{
  sf::Vector2f mv = { 0.f, 0.f };

  bool running = m_is_running;

  
//  if (!OnGround())
  if (false)
    m_is_running = false;
  else {
    if (user_input.GetKeyState(sf::Keyboard::D) == UserInput::RELEASED) {
      mv.x -= m_ms;
      m_is_running = false;
    }
    else if (user_input.GetKeyState(sf::Keyboard::A) == UserInput::RELEASED) {
      mv.x += m_ms;
      m_is_running = false;
    }

    if (user_input.GetKeyState(sf::Keyboard::D) == UserInput::PRESSED) {
      mv.x += m_ms;
      m_is_running = true;
    }
    else if (user_input.GetKeyState(sf::Keyboard::A) == UserInput::PRESSED) {
      mv.x -= m_ms;
      m_is_running = true;
    }

    if (user_input.GetKeyState(sf::Keyboard::Space) == UserInput::PRESSED) {
      mv.y -= 400.f;
    }

    if (mv.x > m_ms)
      mv.x = m_ms;
    else if (mv.x < -m_ms)
      mv.x = -m_ms;
    if (mv.y > m_ms)
      mv.y = m_ms;
    else if (mv.y < -m_ms)
      mv.y = -m_ms;
  }


  SetSpeed(GetSpeed() + mv);
  // printf("%f\n", mv.x);


  if (m_is_running != running) {
    if (m_is_running)
      SetTexture(m_run_anim);
    else
      SetTexture(m_idle_anim);
  }

  RectObject::Update(dt);
}

