#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP


#include <SFML/Graphics.hpp>

#include "system/user-input.hpp"

#include <map>


class Engine
{
public:

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

  class Map;

  class Object;
  class Shape;

  class Player;


  bool m_running;

  sf::VideoMode     m_videomode;
  sf::RenderWindow *m_window;
  sf::View          m_view;

  std::map<std::string, sf::Texture> m_textures;

  Player                 *m_player;
  Shape                  *m_cube;
  sf::Sprite             *m_bg;

  UserInput m_user_input;


  void init_window  ();
  void init_textures();
  void init_map_bg  ();
  void init_player  ();

  void poll_window_events();
  void update_view       ();

  sf::Texture* get_texture(const std::string &name);

  void load_texture(const std::string &name, const std::string fileformat="png");

};


#endif // !ENGINE_ENGINE_HPP
