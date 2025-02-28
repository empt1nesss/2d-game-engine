#include "game.hpp"
#include "engine/objects/objects.hpp"
#include "engine/game/units/player.hpp"
#include "menu.hpp"

#include <cmath>


Engine::Game::Game(const ResourceManager &res_mgr) :
  m_game_state(MENU_STATE),
  m_map(nullptr),
  m_player(nullptr),
  m_menu(nullptr)
{
  m_view.setSize(1600, 900);
  init_menu(res_mgr);
}

Engine::Game::~Game()
{
  if (m_map)
    delete m_map;
  if (m_player)
    delete m_player;
  if (m_menu)
    delete m_menu;
}


void Engine::Game::Update(
  uint64_t               dt,
  const UserInput       &user_input,
  const ResourceManager &rm
)
{
  switch (m_game_state)
  {
  case MENU_STATE:
    update_menu(dt, user_input, rm);
    break;
  case GAME_STATE:
    update_game(dt, user_input, rm);
    break;
  default:
    break;
  }
}

void Engine::Game::Render(sf::RenderTarget &target)
{
  switch (m_game_state)
  {
  case MENU_STATE:
    render_menu(target);
    break;
  case GAME_STATE:
    render_game(target);
    break;
  default:
    break;
  }
}



void Engine::Game::init_menu(const ResourceManager &res_mgr)
{
  if (m_menu)
    delete m_menu;

  m_menu = new Menu(res_mgr);
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
  
  if (m_player)
    delete m_player;

  m_player = new Player(player_idle, player_running, res_mgr);
  m_player->SetPosition(m_map->Spawnpoint);
  m_player->Object().GetSprite().SetFrameTime(100.f);

  update_view(0);
}


void Engine::Game::update_menu(
  uint64_t               dt,
  const UserInput       &user_input,
  const ResourceManager &rm
)
{
  m_menu->Update(rm);

  if (m_menu->IsStartPressed()) {
    m_game_state = GAME_STATE;

    delete m_menu;
    m_menu = nullptr;

    m_map = new Map;
    m_map->Load("map1", rm);

    init_player(rm);
  }
}

void Engine::Game::update_game(
  uint64_t               dt,
  const UserInput       &user_input,
  const ResourceManager &rm
)
{
  if (user_input.GetKeyState(sf::Keyboard::Escape) == UserInput::PRESSED) {
    m_game_state = MENU_STATE;

    delete m_player;
    delete m_map;
    m_player = nullptr;
    m_map = nullptr;

    return;
  }


  m_player->Update(dt, user_input, rm);

  std::vector<Object*> update_objects;
  for (auto &obj : m_map->Objects) {
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
  else if (camera_pos.x > m_map->Size.x - view_size.x / 2.f)
    camera_pos.x = m_map->Size.x - view_size.x / 2.f;
  
  if (camera_pos.y < view_size.y / 2.f)
    camera_pos.y = view_size.y / 2.f;
  else if (camera_pos.y > m_map->Size.y - view_size.y / 2.f)
    camera_pos.y = m_map->Size.y - view_size.y / 2.f;

  m_view.setCenter(camera_pos);
}

void Engine::Game::update_bg(const sf::Vector2f &cam_shift)
{
  for (auto &bg_obj : m_map->BgObjects)
    bg_obj.SetPosition(bg_obj.GetPosition() + cam_shift * bg_obj.GetDepth());
}


void Engine::Game::render_menu(sf::RenderTarget &target)
{
  m_menu->Render(target);
}

void Engine::Game::render_game(sf::RenderTarget &target)
{
  static const auto bin_search = 
  [](int z_index, const std::vector<Object*> &objs)
  {
    size_t i;
    size_t left_i = 0, right_i = objs.size();

    for (;;) {
      i = (left_i + right_i) / 2 + (left_i + right_i) % 2;
      if (i == objs.size()) {
        if (objs.size() > i - 1 && objs[i - 1]->ZIndex > z_index) {
          right_i = i - 1;
          continue;
        }

        break;
      }
      if (i == 0) {
        if (objs.size() > 1 && objs[1]->ZIndex < z_index) {
          left_i = 1;
          continue;
        }

        break;
      }
      
      if (objs[i]->ZIndex < z_index)
        left_i = i;
      else if (objs[i - 1]->ZIndex > z_index)
        right_i = i - 1;
      else
        break;
    }

    return i;
  };

  target.setView(m_view);

  std::vector<Object*> render_bg_objects;
  render_bg_objects.reserve(m_map->BgObjects.size());
  for (auto &obj : m_map->BgObjects) {
    // if object needs to render

    size_t i = bin_search(obj.GetZIndex(), render_bg_objects);
    render_bg_objects.insert(
      render_bg_objects.begin() + i, &obj.GetObject()
    );
  }

  for (auto &bg_obj : render_bg_objects)
    bg_obj->Render(target);

  std::vector<Object*> render_objects;
  render_objects.reserve(
    1 + m_map->Objects.size() + m_map->RuntimeObjects.size()
  );
  render_objects.push_back(&m_player->Object());
  for (auto &obj : m_map->Objects) {
    // if object needs to render

    size_t i = bin_search(obj.ZIndex, render_objects);
    render_objects.insert(render_objects.begin() + i, &obj);
  }
  for (auto &obj : m_map->RuntimeObjects) {
    // if object needs to render

    size_t i = bin_search(obj.second.ZIndex, render_objects);
    render_objects.insert(render_objects.begin() + i, &obj.second);
  }
  
  for (auto &obj : render_bg_objects)
    obj->Render(target);

  for (auto &obj : render_objects)
    obj->Render(target);
}

