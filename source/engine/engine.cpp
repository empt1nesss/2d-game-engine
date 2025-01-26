#include "engine.hpp"
#include "player.hpp"
#include "objects/phys-shape.hpp"


Engine::Engine()
{
  srand(time(0));

  init_window  ();
  init_textures();
  init_map_bg  ();
  init_player  ();

  m_cube = new Shape(sf::VertexArray(sf::TriangleFan, 5));
  
  m_cube->m_vertecies[0] = sf::Vector2f{ 0.f, 0.f };
  m_cube->m_vertecies[1] = sf::Vector2f{ 100.f, 0.f };
  m_cube->m_vertecies[2] = sf::Vector2f{ 100.f, 100.f };
  m_cube->m_vertecies[3] = sf::Vector2f{ 50.f, 150.f };
  m_cube->m_vertecies[4] = sf::Vector2f{ 0.f, 100.f };

  m_cube->m_vertecies[0].color = sf::Color::White;
  m_cube->m_vertecies[1].color = sf::Color::White;
  m_cube->m_vertecies[2].color = sf::Color::White;
  m_cube->m_vertecies[3].color = sf::Color::White;
  m_cube->m_vertecies[4].color = sf::Color::White;

  m_cube->m_v = { .02f, .01f };
  m_cube->m_angular_v = 2.f;

  m_running = true;
}

Engine::~Engine()
{
  delete m_cube;
  delete m_player;
  delete m_bg;
  delete m_window;
}


void Engine::Update(uint64_t dt)
{
  poll_window_events();
  
  auto pl_pos_old = m_player->GetPosition();

  m_player->Update(dt, m_user_input);
  update_view();
  m_cube->Update(dt);
}

void Engine::Render()
{
  m_window->setView(m_view);
  m_window->clear(sf::Color(30, 46, 59));
  m_window->draw(*m_bg);

  m_player->Render(*m_window);

  m_cube->Render(*m_window);

  m_window->display();
}



void Engine::init_window()
{
  m_videomode.width  = 1600;
  m_videomode.height = 900;

  m_window = new sf::RenderWindow(
    m_videomode,
    "First Game",
    sf::Style::Titlebar | sf::Style::Close
  );

  m_view.setSize(1600, 900);
}

void Engine::init_textures()
{
  load_texture("player-idle");
  load_texture("player-running");
  load_texture("bg");
}

void Engine::init_map_bg()
{
  m_bg = new sf::Sprite(*get_texture("bg"));

  m_bg->setOrigin(m_bg->getLocalBounds().getSize() / 2.f);
  m_bg->setPosition(0.f, 0.f);
}

void Engine::init_player()
{
  TextureAtlas player_idle{
    get_texture("player-idle"),
    120,
    80,
    100.f,
    1,
    10
  };
  TextureAtlas player_running{
    get_texture("player-running"),
    120,
    80,
    100.f,
    1,
    10
  };
  m_player = new Player(player_idle, player_running);
}

void Engine::poll_window_events()
{
  m_user_input.Update();

  for (sf::Event wnd_event; m_window->pollEvent(wnd_event);) {
    switch (wnd_event.type)
    {
    case sf::Event::Closed:
      m_window->close();
      m_running = false;
      break;

    default:
      m_user_input.Update(wnd_event, *m_window);
    }
  }
}

void Engine::update_view()
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

sf::Texture* Engine::get_texture(const std::string &name)
{
  auto res = m_textures.find(name);
  if (res == m_textures.end())
    return nullptr;

  return &res->second;
}

void Engine::load_texture(const std::string &name, const std::string fileformat)
{
  sf::Texture player_text;
  player_text.loadFromFile("assets/" + name + "." + fileformat);

  m_textures.emplace(name, std::move(player_text));
}
