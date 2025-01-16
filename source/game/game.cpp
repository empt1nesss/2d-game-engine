#include "game.hpp"
#include "player.hpp"


Game::Game()
{
  srand(time(0));

  init_window  ();
  init_textures();
  init_map_bg  ();
  init_player  ();

  m_running = true;
}

Game::~Game()
{
  delete m_player;
  delete m_bg;
  delete m_window;
}


void Game::Update(uint64_t dt)
{
  poll_window_events();
  
  auto pl_pos_old = m_player->GetPosition();

  m_player->Update(dt, m_keyboard_state);
  update_view();
}

void Game::Render()
{
  m_window->setView(m_view);
  m_window->clear(sf::Color(30, 46, 59));
  m_window->draw(*m_bg);

  m_player->Render(*m_window);

  m_window->display();
}



void Game::init_window()
{
  m_videomode.width  = 1600;
  m_videomode.height = 900;

  m_window = new sf::RenderWindow(
    m_videomode,
    "First Game",
    sf::Style::Titlebar | sf::Style::Close
  );

  m_view.setSize(1600, 900);

  memset(
    m_keyboard_state,
    (int)ButtonState::NOT_PRESSED,
    sf::Keyboard::KeyCount * sizeof(ButtonState)
  );
  memset(
    m_mouse_buttons_state,
    (int)ButtonState::NOT_PRESSED,
    sf::Mouse::ButtonCount * sizeof(ButtonState)
  );
}

void Game::init_textures()
{
  sf::Texture player_text;
  player_text.loadFromFile("assets/player.png");

  m_textures.emplace("player", std::move(player_text));


  sf::Texture bg_text;
  bg_text.loadFromFile("assets/bg.png");

  m_textures.emplace("bg", std::move(bg_text));
}

void Game::init_map_bg()
{
  m_bg = new sf::Sprite(*get_texture("bg"));

  m_bg->setOrigin(m_bg->getLocalBounds().getSize() / 2.f);
  m_bg->setPosition(0.f, 0.f);
}

void Game::init_player()
{
  m_player = new Player(get_texture("player"));
}

void Game::poll_window_events()
{
  for (auto &state : m_keyboard_state) {
    if (state == ButtonState::PRESSED)
      state = ButtonState::SUSTAINED;
    else if (state == ButtonState::RELEASED)
      state = ButtonState::NOT_PRESSED;
  }
  for (auto &state : m_mouse_buttons_state) {
    if (state == ButtonState::PRESSED)
      state = ButtonState::SUSTAINED;
    else if (state == ButtonState::RELEASED)
      state = ButtonState::NOT_PRESSED;
  }


  for (sf::Event wnd_event; m_window->pollEvent(wnd_event);) {
    switch (wnd_event.type)
    {
    case sf::Event::Closed:
      m_window->close();
      m_running = false;
      break;

    case sf::Event::KeyPressed:
      m_keyboard_state[wnd_event.key.code] = ButtonState::PRESSED;
      break;

    case sf::Event::KeyReleased:
      m_keyboard_state[wnd_event.key.code] = ButtonState::RELEASED;
      break;

    case sf::Event::MouseButtonPressed:
      m_mouse_buttons_state[wnd_event.mouseButton.button] = ButtonState::PRESSED;
      break;

    case sf::Event::MouseButtonReleased:
      m_mouse_buttons_state[wnd_event.mouseButton.button] = ButtonState::RELEASED;
      break;

    default:
      break;
    }
  }
}

void Game::update_view()
{
  auto pl_pos     = m_player->GetPosition();
  auto camera_pos = m_player->GetPosition();
  auto bg_bounds     = m_bg->getGlobalBounds();
  auto view_size      = m_view.getSize();

  pl_pos = m_player->GetPosition();

  if (pl_pos.x < bg_bounds.left + view_size.x / 2.f)
    camera_pos.x = bg_bounds.left + view_size.x / 2.f;
  else if (pl_pos.x > bg_bounds.left - view_size.x / 2.f + bg_bounds.width)
    camera_pos.x = bg_bounds.left - view_size.x / 2.f + bg_bounds.width;

  if (pl_pos.y < bg_bounds.top + view_size.y / 2.f)
    camera_pos.y = bg_bounds.top + view_size.y / 2.f;
  else if (pl_pos.y > bg_bounds.top - view_size.y / 2.f + bg_bounds.height)
    camera_pos.y = bg_bounds.top - view_size.y / 2.f + bg_bounds.height;

  if (view_size.x >= bg_bounds.width)
    camera_pos.x = bg_bounds.left + bg_bounds.width / 2.f;
  if (view_size.y >= bg_bounds.height)
    camera_pos.y = bg_bounds.top + bg_bounds.height / 2.f;

  m_view.setCenter(camera_pos);
}

sf::Texture* Game::get_texture(const std::string &name)
{
  auto res = m_textures.find(name);
  if (res == m_textures.end())
    return nullptr;

  return &res->second;
}
