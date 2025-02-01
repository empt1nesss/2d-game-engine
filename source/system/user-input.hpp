#ifndef SYSTEM_USER_INPUT_HPP
#define SYSTEM_USER_INPUT_HPP


#include "SFML/Graphics.hpp"


class UserInput
{
public:
  
  enum ButtonState
  {
    NOT_PRESSED = 0,
    PRESSED     = 1,
    SUSTAINED   = 2,
    RELEASED    = -1
  };

  
  UserInput();
  

  void Update(
    const sf::Event  &event,
    sf::RenderWindow &window
  );
  void Update();

  ButtonState GetKeyState        (sf::Keyboard::Key key) const;
  ButtonState GetMouseButtonState(sf::Mouse::Button button) const;
  
  bool        IsMouseCursorInArea      (sf::FloatRect area) const;
  ButtonState GetMouseButtonStateInArea(
    sf::Mouse::Button button,
    sf::FloatRect     area
  ) const;

  sf::Vector2f GetMousePos() const { return m_mouse_pos_view; }

private:

  ButtonState m_keyboard_state     [sf::Keyboard::KeyCount];
  ButtonState m_mouse_buttons_state[sf::Mouse::ButtonCount];

  sf::Vector2i m_mouse_pos_wnd;
  sf::Vector2f m_mouse_pos_view;

};


#endif // !SYSTEM_USER_INPUT_HPP

