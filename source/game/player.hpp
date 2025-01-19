#ifndef GAME_PLAYER_HPP
#define GAME_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "misc/animated-sprite.hpp"


class Game::Player
{
public:

  Player(Animation idle, Animation run);

  ~Player();


  void Update(uint64_t dt, ButtonState keyboard_state[]);
  void Render(sf::RenderTarget &target);

  sf::Vector2f  GetPosition() const { return m_sprite.getPosition(); }
  sf::FloatRect GetBounds  () const { return m_sprite.getGlobalBounds(); }

  void SetPosition(const sf::Vector2f &pos) { m_sprite.setPosition(pos); }

private:

  Animation m_idle_anim;
  Animation m_run_anim;

  AnimatedSprite m_sprite;

  float m_ms;

  bool m_is_running;

};


#endif // !GAME_PLAYER_HPP
