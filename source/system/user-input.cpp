#include "user-input.hpp"


UserInput::UserInput() :
  m_keyboard_state{},
  m_mouse_buttons_state{}
{
}


void UserInput::Update(
  const sf::Event  &event,
  sf::RenderWindow &window
)
{
  m_mouse_pos_wnd  = sf::Mouse::getPosition(window);
  m_mouse_pos_view = window.mapPixelToCoords(m_mouse_pos_wnd);


  switch (event.type)
  {
  case sf::Event::KeyPressed:
    m_keyboard_state[event.key.code] = PRESSED;
    return;
  
  case sf::Event::KeyReleased:
    m_keyboard_state[event.key.code] = RELEASED;
    return;

  case sf::Event::MouseButtonPressed:
    m_mouse_buttons_state[event.mouseButton.button] = PRESSED;
    return;

  case sf::Event::MouseButtonReleased:
    m_mouse_buttons_state[event.mouseButton.button] = RELEASED;
    return;

  default:
    return;
  }
}

void UserInput::Update()
{
  for (auto &state : m_keyboard_state) {
    if (state == PRESSED)
      state = SUSTAINED;
    else if (state == RELEASED)
      state = NOT_PRESSED;
  }

  for (auto &state : m_mouse_buttons_state) {
    if (state == PRESSED)
      state = SUSTAINED;
    else if (state == RELEASED)
      state = NOT_PRESSED;
  }
}

UserInput::ButtonState UserInput::GetKeyState(
  sf::Keyboard::Key key
) const
{
  if (m_keyboard_state[key] == PRESSED) {
    m_keyboard_state[key] = SUSTAINED;
    return PRESSED;
  }
  else if (m_keyboard_state[key] == RELEASED) {
    m_keyboard_state[key] = NOT_PRESSED;
    return RELEASED;
  }

  return m_keyboard_state[key];
}

UserInput::ButtonState UserInput::GetMouseButtonState(
  sf::Mouse::Button button
) const
{
  if (m_mouse_buttons_state[button] == PRESSED) {
    m_mouse_buttons_state[button] = SUSTAINED;
    return PRESSED;
  }
  else if (m_mouse_buttons_state[button] == RELEASED) {
    m_mouse_buttons_state[button] = NOT_PRESSED;
    return RELEASED;
  }

  return m_mouse_buttons_state[button];
}

bool UserInput::IsMouseCursorInArea(sf::FloatRect area) const
{
  return area.contains(m_mouse_pos_view);
}

UserInput::ButtonState UserInput::GetMouseButtonStateInArea(
  sf::Mouse::Button button,
  sf::FloatRect     area
) const
{
  if (area.contains(m_mouse_pos_view))
    return GetMouseButtonState(button);

  return NOT_PRESSED;
}

