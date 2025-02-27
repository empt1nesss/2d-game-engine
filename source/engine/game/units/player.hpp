#ifndef ENGINE_GAME_UNITS_PLAYER_HPP
#define ENGINE_GAME_UNITS_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "engine/game/game.hpp"
#include "engine/objects/objects.hpp"
#include "system/user-input.hpp"


class Engine::Game::Player : Object
{
public:

  Player(
    TextureAtlas           idle,
    TextureAtlas           run,
    const ResourceManager &rm
  );

  ~Player();


  void Update(
    uint64_t               dt,
    const UserInput       &user_input,
    const ResourceManager &rm
  );
  virtual void Render(sf::RenderTarget &target) override { Object::Render(target); }

  sf::Vector2f  GetPosition () const { return Object::GetPosition(); }
  sf::FloatRect GetBounds   () const { return GetBody().getBounds(); }
  float         GetMoveSpeed() const { return m_ms; }

  Object&             Object()       { return *this; }
  const class Object& Object() const { return *this; }

  void SetPosition(const sf::Vector2f &pos) { Object::SetPosition(pos); }

private:

  TextureAtlas m_idle_anim;
  TextureAtlas m_run_anim;

  float m_ms;

  bool         m_is_running;
  sf::Vector2f m_move_v;

};


#endif // !ENGINE_GAME_UNITS_PLAYER_HPP

