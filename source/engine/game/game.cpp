#include "game.hpp"
#include "engine/objects/objects.hpp"
#include "engine/game/units/player.hpp"

#include <cmath>


Engine::Game::Game(const Map &map, const ResourceManager &res_mgr) : 
  m_map(map)
{
  m_view.setSize(1600, 900);

  init_map_bg();
  init_player(res_mgr);


  m_ground = new RectObject({ 2000.f, 100.f }, { 1000.f, 700.f });
  
  m_ground->DrawBody = true;
  m_ground->EnableCollision(true);
  m_ground->Rotate(0.1f, m_ground->GetPosition());
  m_ground->SetFrictionFactor(0.3f);
  // m_ground->SetRestitutionFactor(0.5f);
  // m_ground->EnableMovement(true);

  m_ground->SetBodyColor(sf::Color::White);


  m_cube = new CircleObject(100.f, { 300.f, 200.f }, 4);
  // m_cube->SetPosition({ 100.f, 200.f });

  m_cube->DrawBody = true;

  m_cube->EnableMovement(true);
  m_cube->EnableRotation(true);
  m_cube->EnableCollision(true);
  m_cube->EnableGravity(true);
  m_cube->SetFrictionFactor(0.3f);
  // m_cube->SetRestitutionFactor(0.5f);

  // m_cube->SetSpeed({ 20.f, 10.f });
  // m_cube->SetAngularSpeed(2.f);
  m_cube->Rotate(0.4f, m_cube->GetPosition());

  m_objects.push_back(m_cube);
  m_objects.push_back(m_ground);
}

Engine::Game::~Game()
{
  m_objects.clear();
  delete m_cube;
  delete m_player;
  delete m_bg;
}


void Engine::Game::Update(uint64_t dt, const UserInput &user_input)
{
  m_player->Update(dt, user_input);
  update_view(dt);
  m_cube->Update(dt);
  m_ground->Update(dt);
  
  Object::ResolveCollision(m_objects);
}

void Engine::Game::Render(sf::RenderTarget &target)
{
  target.setView(m_view);

  target.draw(*m_bg);
  m_ground->Render(target);
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


  // m_objects.push_back(&m_player->Object());
}

void Engine::Game::update_view(uint64_t dt)
{
  const float max_x = 200.f, max_y = 100.f;
  const float min_x = 100.f, min_y = 50.f;

  auto pl_pos     = m_player->GetPosition();
  auto camera_pos = m_view.getCenter();;
  auto view_size  = m_view.getSize();

  auto delta = pl_pos - camera_pos;

  if (delta.x > max_x)
    camera_pos.x += delta.x - max_x;
  else if (delta.x < -max_x)
    camera_pos.x += delta.x + max_x;
  else if (delta.x >= min_x)
    camera_pos.x += (delta.x - min_x) * (dt * 1e-9f * m_player->GetMoveSpeed()) / (max_x - min_x);
  else if (delta.x <= -min_x)
    camera_pos.x += (delta.x + min_x) * (dt * 1e-9f * m_player->GetMoveSpeed()) / (max_x - min_x);

  if (delta.y > max_y)
    camera_pos.y += delta.y - max_y;
  else if (delta.y < -max_y)
    camera_pos.y += delta.y + max_y;
  else if (delta.y >= min_y)
    camera_pos.y += (delta.y - min_y) * (dt * 1e-9f * m_player->GetMoveSpeed()) / (max_y - min_y);
  else if (delta.y <= -min_y)
    camera_pos.y += (delta.y + min_y) * (dt * 1e-9f * m_player->GetMoveSpeed()) / (max_y - min_y);

  if (camera_pos.x < view_size.x / 2.f)
    camera_pos.x = view_size.x / 2.f;
  else if (camera_pos.x > m_map.Size.x - view_size.x / 2.f)
    camera_pos.x = m_map.Size.x - view_size.x / 2.f;
  
  if (camera_pos.y < view_size.y / 2.f)
    camera_pos.y = view_size.y / 2.f;
  else if (camera_pos.y > m_map.Size.y - view_size.y / 2.f)
    camera_pos.y = m_map.Size.y - view_size.y / 2.f;

  m_view.setCenter(camera_pos);
}
