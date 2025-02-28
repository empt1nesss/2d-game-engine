#ifndef ENGINE_GAME_MENU_HPP
#define ENGINE_GAME_MENU_HPP


#include "game.hpp"


class Engine::Game::Menu
{
public:

  Menu(const ResourceManager &rm);

  void Update(const ResourceManager &rm);
  void Render(sf::RenderTarget      &target);

private:



};


#endif // !ENGINE_GAME_MENU_HPP

