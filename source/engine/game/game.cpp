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

  m_objects.emplace("wall1", std::move(RectObject({ 40.f, 500.f }, { 300.f, 400.f })));
  m_objects.emplace("wall2", std::move(RectObject({ 40.f, 500.f }, { 700.f, 400.f })));

  m_objects.emplace("floor1", std::move(RectObject({ 500.f, 40.f }, { 500.f, 200.f })));
  m_objects.emplace("floor2", std::move(RectObject({ 500.f, 40.f }, { 500.f, 600.f })));

  m_objects.emplace("floor3", std::move(RectObject({ 1000.f, 70.f }, { 1400.f, 700.f })));


  m_objects.find("wall1")->second.SetBodyColor(sf::Color::White);
  m_objects.find("wall1")->second.DrawBody = true;
  m_objects.find("wall1")->second.EnableCollision(true);
  m_objects.find("wall1")->second.SetRestitutionFactor(1.f);

  m_objects.find("wall2")->second.SetBodyColor(sf::Color::White);
  m_objects.find("wall2")->second.DrawBody = true;
  m_objects.find("wall2")->second.EnableCollision(true);
  m_objects.find("wall2")->second.SetRestitutionFactor(1.f);

  m_objects.find("floor1")->second.SetBodyColor(sf::Color::White);
  m_objects.find("floor1")->second.DrawBody = true;
  m_objects.find("floor1")->second.EnableCollision(true);
  m_objects.find("floor1")->second.SetRestitutionFactor(1.f);

  m_objects.find("floor2")->second.SetBodyColor(sf::Color::White);
  m_objects.find("floor2")->second.DrawBody = true;
  m_objects.find("floor2")->second.EnableCollision(true);
  m_objects.find("floor2")->second.SetRestitutionFactor(1.f);

  m_objects.find("floor3")->second.SetBodyColor(sf::Color::White);
  m_objects.find("floor3")->second.DrawBody = true;
  m_objects.find("floor3")->second.EnableCollision(true);
  m_objects.find("floor3")->second.SetRestitutionFactor(1.f);
  m_objects.find("floor3")->second.SetFrictionFactor(0.f);
  m_objects.find("floor3")->second.Rotate(0.05, m_objects.find("floor3")->second.GetPosition());

  m_objects.emplace("circle1", std::move(CircleObject(50.f, { 1000.f, 400.f }, 5)));
  m_objects.find("circle1")->second.EnableMovement(true);
  m_objects.find("circle1")->second.EnableCollision(true);
  m_objects.find("circle1")->second.EnableGravity(true);
  m_objects.find("circle1")->second.EnableRotation(true);
  m_objects.find("circle1")->second.Rotate(0.2, m_objects.find("circle1")->second.GetPosition());
  m_objects.find("circle1")->second.SetFrictionFactor(0.f);
  m_objects.find("circle1")->second.DrawBody = true;
}

Engine::Game::~Game()
{}


void Engine::Game::Update(uint64_t dt, const UserInput &user_input)
{
  m_player->Update(dt, user_input);


  if (user_input.GetKeyState(sf::Keyboard::Space) == UserInput::PRESSED) {
    static int count = 1;
    static sf::Color color = sf::Color::Red;

    auto p = m_objects.emplace("ball" + std::to_string(count), std::move(CircleObject(20.f, { 500.f, 300.f })));
    p.first->second.DrawBody = true;
    p.first->second.SetBodyColor(color);
    
    p.first->second.EnableCollision(true);
    p.first->second.EnableMovement(true);
    p.first->second.EnableGravity(true);
    p.first->second.EnableRotation(true);

    p.first->second.SetRestitutionFactor(1.f);
    p.first->second.SetSpeed({ 100.f, 0.f });

    ++count;
    color.b += 50;
  }


  std::vector<Object*> update_objects;
  for (auto &obj : m_objects) {
    // if object needs to update
    obj.second.Update(dt);
    update_objects.push_back(&obj.second);
  }
  update_objects.push_back(&m_player->Object());

  Object::ResolveCollision(update_objects);

  update_view(dt);
}

void Engine::Game::Render(sf::RenderTarget &target)
{
  target.setView(m_view);

  target.draw(*m_bg);

  std::vector<Object*> render_objects { &m_player->Object() };
  for (auto &obj : m_objects) {
    // if object needs to render

    size_t i;
    size_t left_i = 0, right_i = render_objects.size();

    for (;;) {
      i = (left_i + right_i) / 2 + (left_i + right_i) % 2;
      if (i == render_objects.size()) {
        if (render_objects.size() > i - 1 && render_objects[i - 1]->ZIndex > obj.second.ZIndex) {
          right_i = i - 1;
          continue;
        }

        break;
      }
      if (i == 0) {
        if (render_objects.size() > 1 && render_objects[1]->ZIndex < obj.second.ZIndex) {
          left_i = 1;
          continue;
        }

        break;
      }
      
      if (render_objects[i]->ZIndex < obj.second.ZIndex)
        left_i = i;
      else if (render_objects[i - 1]->ZIndex > obj.second.ZIndex)
        right_i = i - 1;
      else
        break;
    }

    render_objects.insert(render_objects.begin() + i, &obj.second);
  }

  for (auto &obj : render_objects)
    obj->Render(target);
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
