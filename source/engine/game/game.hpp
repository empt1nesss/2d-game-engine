#ifndef ENGINE_GAME_GAME
#define ENGINE_GAME_GAME


#include "engine/engine.hpp"
#include "engine/map.hpp"
#include "system/user-input.hpp"
#include "system/resource-manager.hpp"



class Engine::Game
{
public:

  Game(const ResourceManager &res_mgr);

  ~Game();


  void Update(
    uint64_t               dt,
    const UserInput       &user_input,
    const ResourceManager &rm
  );
  void Render(sf::RenderTarget &target);

private:

  class Menu;
  class Player;

  enum GameState
  {
    MENU_STATE,
    GAME_STATE
  };


  GameState m_game_state;

  Map *m_map;

  sf::View m_view;

  Menu   *m_menu;
  Player *m_player;


  void init_menu  (const ResourceManager &res_mgr);
  void init_player(const ResourceManager &res_mgr);

  void update_menu(
    uint64_t               dt,
    const UserInput       &user_input,
    const ResourceManager &rm
  );
  void update_game(
    uint64_t               dt,
    const UserInput       &user_input,
    const ResourceManager &rm
  );
  void update_view(uint64_t            dt);
  void update_bg  (const sf::Vector2f &cam_shift);

  void render_menu(sf::RenderTarget &target);
  void render_game(sf::RenderTarget &target);

};


#endif // !ENGINE_GAME_GAME
