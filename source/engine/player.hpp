#ifndef ENGINE_PLAYER_HPP
#define ENGINE_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "objects/objects.hpp"
#include "system/user-input.hpp"
#include "misc/animated-sprite.hpp"


class Engine::Player : Engine::RectObject
{
public:

  Player(TextureAtlas idle, TextureAtlas run);

  ~Player();


  void Update(uint64_t dt, const UserInput &user_input);
  virtual void Render(sf::RenderTarget &target) override { RectObject::Render(target); }

  sf::Vector2f  GetPosition() const { return RectObject::GetPosition(); }
  sf::FloatRect GetBounds  () const { return GetBody().getBounds(); }

  void SetPosition(const sf::Vector2f &pos) { RectObject::SetPosition(pos); }

private:

  TextureAtlas m_idle_anim;
  TextureAtlas m_run_anim;

  float m_ms;

  bool m_is_running;

};


#endif // !ENGINE_PLAYER_HPP
