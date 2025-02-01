#ifndef ENGINE_GAME_GAME
#define ENGINE_GAME_GAME


#include "engine/engine.hpp"
#include "engine/map.hpp"
#include "system/user-input.hpp"
#include "system/resource-manager.hpp"


// This class must contains play time stuff like
// current map, player, and other


class Engine::Game
{
public:

  Game(const Map &map, const ResourceManager &res_mgr);

  ~Game();


  void Update(uint64_t dt, const UserInput &user_input);
  void Render(sf::RenderTarget &target);

private:

  Map m_map;

  sf::View m_view;

  Player     *m_player;
  Shape      *m_cube;
  sf::Sprite *m_bg;


  void init_map_bg();
  void init_player(const ResourceManager &res_mgr);

  void update_view();

};


#endif // !ENGINE_GAME_GAME
