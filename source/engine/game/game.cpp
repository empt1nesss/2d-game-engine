#include "game.hpp"
#include "engine/objects/objects.hpp"
#include "engine/player.hpp"


Engine::Game::Game(const Map &map, const ResourceManager &res_mgr) : 
  m_map(map)
{
  m_view.setSize(1600, 900);

  init_map_bg();
  init_player(res_mgr);

  m_cube = new CircleObject(100.f, { 1000.f, 200.f });
  // m_cube->SetPosition({ 100.f, 200.f });

  m_cube->DrawBody = true;

  m_cube->EnableMovement = true;
  m_cube->EnableRotation = true;
  m_cube->EnableGravity = true;

  m_cube->Speed = { 200.f, 100.f };
  m_cube->AngularSpeed = 1.f;
}

Engine::Game::~Game()
{
  delete m_cube;
  delete m_player;
  delete m_bg;
}


void Engine::Game::Update(uint64_t dt, const UserInput &user_input)
{
  m_player->Update(dt, user_input);
  update_view();
  m_cube->Update(dt);
}

void Engine::Game::Render(sf::RenderTarget &target)
{
  target.setView(m_view);

  target.draw(*m_bg);
  m_player->Render(target);
  m_cube->Render(target);
}



void Engine::Game::init_map_bg()
{
  m_bg = new sf::Sprite(m_map.BgTexture);
}

void Engine::Game::init_player(const ResourceManager &res_mgr)
{
  TextureAtlas player_idle{
    res_mgr.GetTexture("player-idle"),
    120,
    80,
    100.f,
    1,
    10
  };
  TextureAtlas player_running{
    res_mgr.GetTexture("player-running"),
    120,
    80,
    100.f,
    1,
    10
  };
  m_player = new Player(player_idle, player_running);
  m_player->SetPosition(m_map.Spawnpoint);
}

void Engine::Game::update_view()
{
  auto pl_pos     = m_player->GetPosition();
  auto camera_pos = m_player->GetPosition();
  auto view_size  = m_view.getSize();

  if (pl_pos.x < view_size.x / 2.f)
    camera_pos.x = view_size.x / 2.f;
  else if (pl_pos.x > -view_size.x / 2.f + m_map.Size.x)
    camera_pos.x = -view_size.x / 2.f + m_map.Size.x;

  if (pl_pos.y < view_size.y / 2.f)
    camera_pos.y = view_size.y / 2.f;
  else if (pl_pos.y > -view_size.y / 2.f + m_map.Size.y)
    camera_pos.y = -view_size.y / 2.f + m_map.Size.y;

  if (view_size.x >= m_map.Size.x)
    camera_pos.x = m_map.Size.x / 2.f;
  if (view_size.y >= m_map.Size.y)
    camera_pos.y = m_map.Size.y / 2.f;

  m_view.setCenter(camera_pos);
}

