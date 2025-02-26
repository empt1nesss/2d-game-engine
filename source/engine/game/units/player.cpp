#include "player.hpp"

#include <cmath>


Engine::Game::Player::Player(
  TextureAtlas           idle,
  TextureAtlas           run,
  const ResourceManager &rm
) :
  Engine::Object(std::move(Object::CreateRectObj({ 80.f, 154.f }))),
  m_ms(500.f),
  m_idle_anim(idle),
  m_run_anim(run)
{
  // SetBodyColor(sf::Color(255, 0, 0, 127));
  GetSprite().SwitchAnimation(idle, rm);
  GetSprite().setScale(sf::Vector2f{ 4.f, 4.f });
  GetSprite().setOrigin({ 54.5f, 62.f });
  SetMass(60.f);
  SetRestitutionFactor(0.f);
  SetFrictionFactor(1.f);

  EnableMovement(true);
  EnableGravity(true);
  // EnableRotation(true);
  EnableCollision(true);

  // DrawBody = true;

  // SetSpeed({ 100.f, 350.f });

  // Rotate(1.f, GetPosition());
}

Engine::Game::Player::~Player()
{

}


void Engine::Game::Player::Update(
  uint64_t               dt,
  const UserInput       &user_input,
  const ResourceManager &rm
)
{
  static bool falling = true;
  sf::Vector2f mv = { 0.f, 0.f };
  
  
  if (user_input.GetKeyState(sf::Keyboard::D) > 0)
    mv.x += 1.f;
  if (user_input.GetKeyState(sf::Keyboard::A) > 0)
    mv.x -= 1.f;
  // if (user_input.GetKeyState(sf::Keyboard::S) > 0)
  //   mv.y += 1.f;
  // if (user_input.GetKeyState(sf::Keyboard::W) > 0)
  //   mv.y -= 1.f;

  mv = normalize(mv) * m_ms;

  if (user_input.GetKeyState(sf::Keyboard::Space) == UserInput::PRESSED && OnGround())
    mv.y = -1000.f;

  if (mv.x != m_move_v.x || falling) {
    if (mv.x == 0.f) {
      SetFrictionFactor(1.f);
      GetSprite().SwitchAnimation(m_idle_anim, rm);
    }
    else {
      SetFrictionFactor(0.f);
      GetSprite().SwitchAnimation(m_run_anim, rm);
    }

    if (mv.x < 0)
      GetSprite().setScale({ -4.f, 4.f});
    else 
      GetSprite().setScale({ 4.f, 4.f});
  }
  falling = false;

  if (mv != m_move_v) {
    auto v = GetSpeed();
    if (std::abs(v.x) < std::abs(m_move_v.x))
      m_move_v.x = v.x;
    else if ((v.x > 0.f) != (m_move_v.x > 0.f))
      m_move_v.x *= -1.f;

    SetSpeed(GetSpeed() - m_move_v + mv);
    m_move_v.x = mv.x;
  }

  const float gravity_correction = 0.1f;
  if (!IsGravityEnabled())
    Move({ 0.f, gravity_correction });


  Object::Update(dt);

  EnableGravity(!OnGround());
}

