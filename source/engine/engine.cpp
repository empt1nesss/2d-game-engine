#include "engine.hpp"
#include "engine/map.hpp"
#include "engine/game/game.hpp"


float Engine::GravityAcceleration = 980.f;


Engine::Engine() : m_game(nullptr)
{
  srand(time(0));

  init_window  ();
  init_textures();
  init_game    ();

  m_running = true;
}

Engine::~Engine()
{
  delete m_game;
  delete m_window;
}


void Engine::Update(uint64_t dt)
{
  poll_window_events();
  
  m_game->Update(dt, m_user_input);
}

void Engine::Render()
{
  m_window->clear(sf::Color(30, 46, 59));

  m_game->Render(*m_window);

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
}

void Engine::init_textures()
{
  m_resource_manager.LoadTexture("player-idle");
  m_resource_manager.LoadTexture("player-running");
  m_resource_manager.LoadTexture("bg");
}

void Engine::init_game()
{
  if (m_game)
    delete m_game;

  Map map;
  map.Size = { 2000, 2000 };
  map.Spawnpoint = { 100, 500 };
  map.BgObjects.emplace(std::move(Map::BgObject(*m_resource_manager.GetTexture("bg"), 0.5)));

  m_game = new Game(map, m_resource_manager);
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

