#include "game.hpp"
#include "engine/objects/objects.hpp"
#include "engine/game/units/player.hpp"

#include <cmath>


Engine::Game::Game(const Map &map, const ResourceManager &res_mgr) : 
  m_map(map)
{
  m_view.setSize(1600, 900);

  init_player(res_mgr);
  update_view(0);
}

Engine::Game::~Game()
{}


void Engine::Game::Update(
  uint64_t               dt,
  const UserInput       &user_input,
  const ResourceManager &rm
)
{
  m_player->Update(dt, user_input, rm);

  std::vector<Object*> update_objects;
  for (auto &obj : m_map.Objects) {
    // if object needs to update
    obj.Update(dt);
    update_objects.push_back(&obj);
  }
  update_objects.push_back(&m_player->Object());

  Object::ResolveCollision(update_objects);

  auto cam_pos = m_view.getCenter();
  update_view(dt);
  update_bg(m_view.getCenter() - cam_pos);
}

void Engine::Game::Render(sf::RenderTarget &target)
{
  target.setView(m_view);

  for (auto &bg_obj : m_map.BgObjects)
    bg_obj.Render(target);

  std::vector<Object*> render_objects { &m_player->Object() };
  for (auto &obj : m_map.Objects) {
    // if object needs to render

    size_t i;
    size_t left_i = 0, right_i = render_objects.size();

    for (;;) {
      i = (left_i + right_i) / 2 + (left_i + right_i) % 2;
      if (i == render_objects.size()) {
        if (render_objects.size() > i - 1 && render_objects[i - 1]->ZIndex > obj.ZIndex) {
          right_i = i - 1;
          continue;
        }

        break;
      }
      if (i == 0) {
        if (render_objects.size() > 1 && render_objects[1]->ZIndex < obj.ZIndex) {
          left_i = 1;
          continue;
        }

        break;
      }
      
      if (render_objects[i]->ZIndex < obj.ZIndex)
        left_i = i;
      else if (render_objects[i - 1]->ZIndex > obj.ZIndex)
        right_i = i - 1;
      else
        break;
    }

    render_objects.insert(render_objects.begin() + i, &obj);
  }

  for (auto &obj : render_objects)
    obj->Render(target);
}



void Engine::Game::init_player(const ResourceManager &res_mgr)
{
  TextureAtlas player_idle{
    "player-idle",
    120,
    80,
    1,
    10
  };
  TextureAtlas player_running{
    "player-running",
    120,
    80,
    1,
    10
  };
  m_player = new Player(player_idle, player_running, res_mgr);
  m_player->SetPosition(m_map.Spawnpoint);
  m_player->Object().GetSprite().SetFrameTime(100.f);
}

void Engine::Game::update_view(uint64_t dt)
{
  const float max_x    = 200.f, max_y = 100.f;
  const float min_x    = 100.f, min_y = 50.f;
  // const float border_x = 200.f, border_y = 100.f;

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

void Engine::Game::update_bg(const sf::Vector2f &cam_shift)
{
  for (auto &bg_obj : m_map.BgObjects)
    bg_obj.SetPosition(bg_obj.GetPosition() + cam_shift * bg_obj.GetDepth());
}
