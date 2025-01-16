#ifndef GAME_PLAYER_HPP
#define GAME_PLAYER_HPP


#include <SFML/Graphics.hpp>

#include "game.hpp"


class Game::Player
{
public:

  Player(sf::Texture *texture);

  ~Player();


  void Update(uint64_t dt, ButtonState keyboard_state[]);
  void Render(sf::RenderTarget &target);

  sf::Vector2f  GetPosition() const { return m_sprite.getPosition(); }
  sf::FloatRect GetBounds  () const { return m_sprite.getGlobalBounds(); }

  void SetPosition(const sf::Vector2f &pos) { m_sprite.setPosition(pos); }

private:

  sf::Sprite m_sprite;

  float m_ms;

};


#endif // !GAME_PLAYER_HPP
