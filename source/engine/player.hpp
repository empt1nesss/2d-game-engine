#ifndef ENGINE_PLAYER_HPP
#define ENGINE_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "system/user-input.hpp"
#include "misc/animated-sprite.hpp"


class Engine::Player
{
public:

  Player(TextureAtlas idle, TextureAtlas run);

  ~Player();


  void Update(uint64_t dt, const UserInput &user_input);
  void Render(sf::RenderTarget &target);

  sf::Vector2f  GetPosition() const { return m_sprite.getPosition(); }
  sf::FloatRect GetBounds  () const { return m_sprite.getGlobalBounds(); }

  void SetPosition(const sf::Vector2f &pos) { m_sprite.setPosition(pos); }

private:

  TextureAtlas m_idle_anim;
  TextureAtlas m_run_anim;

  AnimatedSprite m_sprite;

  float m_ms;

  bool m_is_running;

};


#endif // !ENGINE_PLAYER_HPP
