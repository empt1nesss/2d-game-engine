#ifndef ENGINE_GAME_UNITS_PLAYER_HPP
#define ENGINE_GAME_UNITS_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "engine/game/game.hpp"
#include "engine/objects/objects.hpp"
#include "system/user-input.hpp"
#include "misc/animated-sprite.hpp"


class Engine::Game::Player : Engine::RectObject
{
public:

  Player(TextureAtlas idle, TextureAtlas run);

  ~Player();


  void Update(uint64_t dt, const UserInput &user_input);
  virtual void Render(sf::RenderTarget &target) override { RectObject::Render(target); }

  sf::Vector2f  GetPosition () const { return RectObject::GetPosition(); }
  sf::FloatRect GetBounds   () const { return GetBody().getBounds(); }
  float         GetMoveSpeed() const { return m_ms; }

  Object&             Object()       { return *this; }
  const class Object& Object() const { return *this; }

  void SetPosition(const sf::Vector2f &pos) { RectObject::SetPosition(pos); }

private:

  TextureAtlas m_idle_anim;
  TextureAtlas m_run_anim;

  float m_ms;

  sf::Vector2f m_move_v;

};


#endif // !ENGINE_GAME_UNITS_PLAYER_HPP

