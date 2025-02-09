#include "player.hpp"

#include <cmath>


Engine::Game::Player::Player(TextureAtlas idle, TextureAtlas run) :
  Engine::RectObject({ 80.f, 154.f }),
  m_ms(500.f),
  m_idle_anim(idle),
  m_run_anim(run)
{
  // SetBodyColor(sf::Color(255, 0, 0, 127));
  SetTexture(idle);
  SetSpriteScale(sf::Vector2f{ 4.f, 4.f });
  SetSpriteOrigin({ 54.5f, 62.f });
  SetMass(100.f);
  SetFrictionFactor(1.f);

  EnableMovement(true);
  EnableGravity(true);
  EnableCollision(true);

  // DrawBody = true;

  // SetSpeed({ 250.f, -50.f });
}

Engine::Game::Player::~Player()
{

}


void Engine::Game::Player::Update(uint64_t dt, const UserInput &user_input)
{
  static bool falling = true;
  sf::Vector2f mv = { 0.f, 0.f };
  
  
  if (user_input.GetKeyState(sf::Keyboard::D) > 0)
    mv.x += 1.f;
  if (user_input.GetKeyState(sf::Keyboard::A) > 0)
    mv.x -= 1.f;
  if (user_input.GetKeyState(sf::Keyboard::Space) == UserInput::PRESSED && OnGround())
    mv.y = -20.f;
  // if (user_input.GetKeyState(sf::Keyboard::S) > 0)
  //   mv.y += 1.f;
  // if (user_input.GetKeyState(sf::Keyboard::W) > 0)
  //   mv.y -= 1.f;

  mv = normalize(mv) * m_ms;

  if (mv.x != m_move_v.x || falling) {
    if (mv.x == 0.f) {
      SetFrictionFactor(1.f);
      SetTexture(m_idle_anim);
    }
    else {
      SetFrictionFactor(0.f);
      SetTexture(m_run_anim);
    }

    if (mv.x < 0)
      SetSpriteScale({ -4.f, 4.f});
    else 
      SetSpriteScale({ 4.f, 4.f});
  }
  falling = false;

  if (mv != m_move_v) {
    SetSpeed(GetSpeed() - m_move_v + mv);
    m_move_v.x = mv.x;
  }

  // printf("\r%f        ", GetFr)

  RectObject::Update(dt);
}

