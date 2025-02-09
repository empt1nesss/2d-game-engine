#ifndef ENGINE_GAME_GAME
#define ENGINE_GAME_GAME


#include "engine/engine.hpp"
#include "engine/map.hpp"
#include "system/user-input.hpp"
#include "system/resource-manager.hpp"



class Engine::Game
{
public:

  Game(const Map &map, const ResourceManager &res_mgr);

  ~Game();


  void Update(uint64_t dt, const UserInput &user_input);
  void Render(sf::RenderTarget &target);

private:

  class Player;


  Map m_map;

  sf::View m_view;

  Player *m_player;


  void init_player(const ResourceManager &res_mgr);

  void update_view(uint64_t            dt);
  void update_bg  (const sf::Vector2f &cam_shift);

};


#endif // !ENGINE_GAME_GAME
