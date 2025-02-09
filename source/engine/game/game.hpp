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
  
  std::map<std::string, Object> m_objects;

  Player      *m_player;
  sf::Sprite  *m_bg;


  void init_map_bg();
  void init_player(const ResourceManager &res_mgr);

  void update_view(uint64_t dt);

};


#endif // !ENGINE_GAME_GAME
