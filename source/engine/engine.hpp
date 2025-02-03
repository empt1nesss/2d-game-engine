#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP


#include <SFML/Graphics.hpp>

#include "system/user-input.hpp"
#include "system/resource-manager.hpp"


class Engine
{
public:
  
  // Must be non-static in future
  static float GravityAcceleration;


  Engine();

  ~Engine();


  void Update(uint64_t dt);
  void Render();

  bool IsRunning() const { return m_running; }

private:

  enum class ButtonState
  {
    NOT_PRESSED = 0,
    PRESSED     = 1,
    SUSTAINED   = 2,
    RELEASED    = -1,
  };

  struct Map;

  class Object;
  class SpriteObject;
  class ShapeObject;

  class Game;
  class Player;


  bool m_running;

  sf::VideoMode     m_videomode;
  sf::RenderWindow *m_window;

  UserInput       m_user_input;
  ResourceManager m_resource_manager;

  Game *m_game;


  void init_window  ();
  void init_textures();
  void init_game    ();

  void poll_window_events();

};


#endif // !ENGINE_ENGINE_HPP
