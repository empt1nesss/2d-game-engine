#include "objects.hpp"

#include <cmath>
#include <corecrt_math_defines.h>


void Engine::Object::ResolveCollision(const std::vector<Object*> objects)
{
  for (size_t i = 0; i < objects.size(); ++i) {
    if (!objects[i]->IsCollisionEnabled())
      continue;

    for (size_t j = i + 1; j < objects.size(); ++j) {
      if (!objects[j]->IsCollisionEnabled())
        continue;

      if (!objects[i]->IsMovementEnabled() && !objects[j]->IsMovementEnabled())
        continue;

      auto collision_info = objects[i]->get_collision_info(*objects[j]);
      if (!collision_info.collision_detected)
        continue;

      float        correction_factor = 2.f;
      sf::Vector2f correction        = (
        // normalize(collision_info.normal) * (collision_info.depth * correction_factor)
        collision_info.normal * (collision_info.depth * correction_factor)
      );
      
      if (objects[i]->IsMovementEnabled())
        objects[i]->SetPosition(objects[i]->GetPosition() + correction);
      else
        objects[j]->SetPosition(objects[j]->GetPosition() - correction);

      if (objects[i]->GetRestitution() == 0.f && objects[j]->GetRestitution() == 0.f) {
        objects[i]->SetSpeed({ 0.f, 0.f });
        objects[j]->SetSpeed({ 0.f, 0.f });
        continue;
      }

      float m1 = objects[i]->GetMass(), m2 = objects[j]->GetMass();

      sf::Vector2f n = normalize(collision_info.normal);
      sf::Vector2f t = perpendicular(n);
      
      float v1n = dot(objects[i]->GetSpeed(), n), v1t = dot(objects[i]->GetSpeed(), t);
      float v2n = dot(objects[j]->GetSpeed(), n), v2t = dot(objects[j]->GetSpeed(), t);

      float v1n_after = ((m1 - m2) * v1n + 2.f * m2 * v2n) / (m1 + m2);
      float v2n_after = ((m2 - m1) * v2n + 2.f * m1 * v1n) / (m1 + m2);


      float avg_r = (objects[i]->GetRestitution() + objects[j]->GetRestitution()) / 2.f;

      if (!objects[i]->IsMovementEnabled()) {
        objects[j]->SetSpeed(
          (objects[j]->GetSpeed() - n * (2.f * v2n)) * avg_r
        );
      }
      else if (!objects[j]->IsMovementEnabled()) {
        objects[i]->SetSpeed(
          (objects[i]->GetSpeed() - n * (2.f * v1n)) * avg_r
        );
      }
      else {
        objects[i]->SetSpeed((n * v1n_after + t * v1t) * avg_r);
        objects[j]->SetSpeed((n * v2n_after + t * v2t) * avg_r);
      }
    }
  }
}



Engine::Object::Object(const std::vector<sf::Vector2f> &vertices, sf::Vector2f pos) :
  m_enable_collision(false),
  m_enable_movement (false),
  m_enable_gravity  (false),
  m_enable_rotation (false),

  m_v          ({ 0.f, 0.f }),
  m_av         (0.f),
  m_m          (1.f),
  m_restitution(0.f),

  DrawBody(false)
{
  m_body.setPrimitiveType(sf::TriangleFan);

  m_body.resize(vertices.size());
  for (size_t i = 0; i < vertices.size(); ++i) {
    m_body[i].position = vertices[i];
    m_body[i].color    = sf::Color::Red;
    m_center += vertices[i];
  }

  m_center /= (float)vertices.size();

  SetPosition(pos);

  m_on_ground = is_on_ground();
}


void Engine::Object::Update(uint64_t dt)
{
  m_sprite.Update(dt);

  if (m_enable_movement) {
    if (m_enable_gravity) {
      m_v.y += (dt * 1e-9f * GravityAcceleration);
    }

    sf::Vector2f s = m_v * (dt * 1e-9f);
    for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
      m_body[i].position += s;
    }
    m_center += s;
    m_sprite.move(s);
  }
  if (m_enable_rotation) {
    float as = m_av * (dt * 1e-9f);

    for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
      m_body[i].position -= m_center;

      m_body[i].position = {
        (float)(cos(as) * m_body[i].position.x +
        -sin(as) * m_body[i].position.y),

        (float)(sin(as) * m_body[i].position.x +
        cos(as) * m_body[i].position.y)
      };

      m_body[i].position += m_center;
    }

    m_sprite.rotate(as);
  }

  m_on_ground = is_on_ground();
}

void Engine::Object::Render(sf::RenderTarget &target)
{
  target.draw(m_sprite);
  if (DrawBody)
    target.draw(m_body);
}

void Engine::Object::EnableCollision(bool state)
{
  m_enable_collision = state;
}

void Engine::Object::EnableMovement(bool state)
{
  if (m_enable_movement == state)
    return;

  m_enable_movement = state;

  if (!m_enable_movement)
    m_v = { 0.f, 0.f };
}

void Engine::Object::EnableGravity(bool state)
{
  m_enable_gravity = state;
}

void Engine::Object::EnableRotation(bool state)
{
  if (m_enable_rotation == state)
    return;

  m_enable_rotation = state;

  if (!m_enable_rotation)
    m_av = 0.f;
}

bool Engine::Object::SetSpeed(const sf::Vector2f &v)
{
  if (!m_enable_movement)
    return false;

  m_v = v;
  return true;
}

bool Engine::Object::SetAngularSpeed(float av)
{
  if (!m_enable_rotation)
    return false;

  m_av = av;
  return true;
}

bool Engine::Object::SetMass(float m)
{
  if (m <= 0.f)
    return false;

  m_m = m;
  return true;
}

bool Engine::Object::SetRestitution(float val)
{
  if (val < 0.f) {
    m_restitution = 0.f;
    return false;
  }
  else if (val > 1.f) {
    m_restitution = 1.f;
    return false;
  }

  m_restitution = val;
  return true;
}

void Engine::Object::SetPosition(sf::Vector2f pos)
{
  auto s = pos - m_center;
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;

  m_center = pos;

  m_sprite.setPosition(pos);
}

void Engine::Object::Move(sf::Vector2f s)
{
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;
  
  m_center += s;

  m_sprite.move(s);
}

void Engine::Object::SetTexture(TextureAtlas texture_atlas)
{
  m_sprite.SwitchAnimation(texture_atlas);
  m_sprite.setPosition(m_center);
}

void Engine::Object::SetSpriteScale(sf::Vector2f scale)
{
  m_sprite.setScale(scale);
}

void Engine::Object::SetSpriteOrigin(sf::Vector2f origin)
{
  m_sprite.setOrigin(origin);
  m_sprite.setPosition(m_center);
}

void Engine::Object::SetSpriteRotation(float angle)
{
  m_sprite.setRotation(angle);
}

void Engine::Object::SetBodyColor(sf::Color color)
{
  for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
    m_body[i].color = color;
  }
}


bool lines_intersects(
  sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2
)
{
  auto cross = [](double x1, double y1, double x2, double y2)
  {
    return x1 * y2 - y1 * x2;
  };

  double d1, d2, d3, d4;

  d1 = cross(A2.x - A1.x, A2.y - A1.y, B1.x - A1.x, B1.y - A1.y);
  d2 = cross(A2.x - A1.x, A2.y - A1.y, B2.x - A1.x, B2.y - A1.y);
  d3 = cross(B2.x - B1.x, B2.y - B1.y, A1.x - B1.x, A1.y - B1.y);
  d4 = cross(B2.x - B1.x, B2.y - B1.y, A2.x - B1.x, A2.y - B1.y);

  return (
    ((d1 > 0. && d2 < 0.) || (d1 < 0. && d2 > 0.)) &&
    ((d3 > 0. && d4 < 0.) || (d3 < 0. && d4 > 0.))
  );
}

bool Engine::Object::IsIntersects(const Object &object) const
{
  if (!m_body.getBounds().intersects(object.m_body.getBounds()))
    return false;

  size_t n1 = m_body.getVertexCount();
  size_t n2 = object.m_body.getVertexCount();

  for (size_t i = 0; i < n1; ++i) {
    for (size_t j = 0; j < n2; ++j) {
      sf::Vector2f A, B, C, D;

      A = m_body[i].position;
      B = m_body[(i + 1) % n1].position;

      C = object.m_body[j].position;
      D = object.m_body[(j + 1) % n2].position;


      if (lines_intersects(A, B, C, D))
        return true;
    }
  }
  
  return false;
}

bool Engine::Object::IsContains(const sf::Vector2f &point) const
{
  bool inside = false;
  size_t n = m_body.getVertexCount();
  
  for (size_t i = 0, j = n - 1; i < n; j = i++) {
    double xi = m_body[i].position.x, yi = m_body[i].position.y;
    double xj = m_body[j].position.x, yj = m_body[j].position.y;
    
    bool intersect = (
      ((yi > point.y) != (yj > point.y)) &&
      (point.x < (xj - xi) * (point.y - yi) / (yj - yi) + xi)
    );
    
    if (intersect)
      inside = !inside;
  }
  
  return inside;
}



bool Engine::Object::is_on_ground()
{
  if (!m_enable_movement)
    return false;

  if (!m_enable_gravity)
    return true;
  
  // sf::Vector2f under_point = m_body[0].position;
  // for (size_t i = 1; i < m_body.getVertexCount(); ++i)
  //   if (m_body[i].position.y > under_point.y)
  //     under_point = m_body[i].position;

  // for (auto &object : m_objects) {
  //   if (this != object && object->m_enable_collision)
  //     if (object->IsContains(under_point))
  //       return true;
  // }

  return false;
}

Engine::Object::CollisionInfo Engine::Object::get_collision_info(
  const Object &object
) const
{
  float        min_overlap = INFINITY;
  sf::Vector2f best_normal;

  auto get_edges = [](const sf::VertexArray &poly)
  {
    std::vector<sf::Vector2f> edges;
    for (size_t i = 0; i < poly.getVertexCount(); ++i) {
      sf::Vector2f v1 = poly[i].position;
      sf::Vector2f v2 = poly[(i + 1) % poly.getVertexCount()].position;

      edges.push_back(v2 - v1);
    }
    return edges;
  };

  auto project_polygon = [](const sf::VertexArray &poly, sf::Vector2f axis)
  {
    float min_proj = INFINITY, max_proj = -INFINITY;

    for (size_t i = 0; i < poly.getVertexCount(); ++i) {
      float proj = dot(poly[i].position, axis);
      if (proj < min_proj)
        min_proj = proj;
      if (proj > max_proj)
        max_proj = proj;
    }

    return std::make_pair(min_proj, max_proj);
  };

  auto edges1 = get_edges(m_body), edges2 = get_edges(object.m_body);
  
  for (auto &edge : edges1) {
    sf::Vector2f axis = normalize(perpendicular(edge));
    auto [min1, max1] = project_polygon(m_body, axis);
    auto [min2, max2] = project_polygon(object.m_body, axis);

    float overlap = std::min(max1, max2) - std::max(min1, min2);

    if (overlap <= 0.f)
      return { false, { 0.f, 0.f }, 0.f  };

    if (overlap < min_overlap) {
      min_overlap = overlap;
      best_normal = axis;
    }
  }

  for (auto &edge : edges2) {
    sf::Vector2f axis = normalize(perpendicular(edge));
    auto [min1, max1] = project_polygon(m_body, axis);
    auto [min2, max2] = project_polygon(object.m_body, axis);

    float overlap = std::min(max1, max2) - std::max(min1, min2);

    if (overlap <= 0.f)
      return { false, { 0.f, 0.f }, 0.f };

    if (overlap < min_overlap) {
      min_overlap = overlap;
      best_normal = axis;
    }
  }

  sf::Vector2f direction = GetPosition() - object.GetPosition();
  if (dot(best_normal, direction) < 0)
    best_normal = -best_normal;


  return { true, best_normal, min_overlap };
}




Engine::RectObject::RectObject(sf::Vector2f size, sf::Vector2f pos) :
  Object({
    { 0.f,    0.f    },
    { size.x, 0.f    },
    { size.x, size.y },
    { 0.f,    size.y }
  }, pos)
{}




Engine::CircleObject::CircleObject(float radius, sf::Vector2f pos, unsigned vertex_count) :
  Object(create_circle(radius, vertex_count), pos)
{}



std::vector<sf::Vector2f> Engine::CircleObject::create_circle(
  float radius, unsigned vertex_count
)
{
  std::vector<sf::Vector2f> res(vertex_count);

  for (unsigned v = 0; v < vertex_count; ++v) {
    float angle = 2.f * M_PI / vertex_count * v;
    res[v] = {
      (float)(sin(angle) * radius),
      (float)(cos(angle) * radius)
    };
  }

  return res;
}

