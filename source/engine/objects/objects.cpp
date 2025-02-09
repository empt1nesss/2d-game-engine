#include "objects.hpp"

#include <cmath>
#include <corecrt_math_defines.h>


void Engine::Object::ResolveCollision(const std::vector<Object*> objects)
{
  for (auto &p_obj : objects)
    p_obj->m_in_contact = false;


  for (size_t i = 0; i < objects.size(); ++i) {
    if (!objects[i]->m_enable_collision)
      continue;

    for (size_t j = i + 1; j < objects.size(); ++j) {
      if (!objects[j]->m_enable_collision)
        continue;

      if (
        !objects[i]->m_enable_movement && !objects[i]->m_enable_rotation &&
        !objects[j]->m_enable_movement && !objects[j]->m_enable_rotation
      ) {
        continue;
      }


      auto collision_info = objects[i]->get_collision_info(*objects[j]);
      if (!collision_info.collision_detected)
        continue;

      objects[i]->m_in_contact = true;
      objects[j]->m_in_contact = true;


      float inv_m1 = (objects[i]->m_m > 0.f) ? 1.f / objects[i]->m_m : 0.f;
      float inv_m2 = (objects[j]->m_m > 0.f) ? 1.f / objects[j]->m_m : 0.f;
      float total_inv_m = inv_m1 + inv_m2;

      if (total_inv_m == 0.f)
        continue;

      sf::Vector2f correction = collision_info.normal * (collision_info.depth / total_inv_m);
      objects[i]->Move(correction * inv_m1);
      objects[j]->Move(-correction * inv_m2);


      sf::Vector2f contact_point{ 0.f, 0.f };
      for (const auto &p : collision_info.contact_points)
        contact_point += p;
      contact_point /= (float)(collision_info.contact_points.size());

      sf::Vector2f r1 = contact_point - objects[i]->m_mass_center;
      sf::Vector2f r2 = contact_point - objects[j]->m_mass_center;

      sf::Vector2f v1_contact = objects[i]->m_v + objects[i]->m_av * perpendicular(r1);
      sf::Vector2f v2_contact = objects[j]->m_v + objects[j]->m_av * perpendicular(r2);

      sf::Vector2f rel_v = v1_contact - v2_contact;

      float v_along_normal = dot(rel_v, collision_info.normal);
      if (v_along_normal > 0.f)
        continue;

      float e = std::min(objects[i]->m_e, objects[j]->m_e);

      float r1_cross_n = cross(r1, collision_info.normal);
      float r2_cross_n = cross(r2, collision_info.normal);

      float inv_I1 = (objects[i]->m_I > 0.f) ? 1.f / objects[i]->m_I : 0.f;
      float inv_I2 = (objects[j]->m_I > 0.f) ? 1.f / objects[j]->m_I : 0.f;

      float _j = -(1.f + e) * v_along_normal;
      _j /= (inv_m1 + inv_m2 + (r1_cross_n * r1_cross_n) * inv_I1 + (r2_cross_n * r2_cross_n) * inv_I2);

      sf::Vector2f impulse = _j * collision_info.normal;


      objects[i]->m_v += impulse * inv_m1;
      objects[j]->m_v -= impulse * inv_m2;

      objects[i]->m_av += cross(r1, impulse) * inv_I1;
      objects[j]->m_av -= cross(r2, impulse) * inv_I2;


      sf::Vector2f tan = rel_v - collision_info.normal * dot(rel_v, collision_info.normal);
      float tan_len = std::sqrt(dot(tan, tan));
      if (tan_len > std::numeric_limits<float>::epsilon())
        tan /= tan_len;
      else
        tan = sf::Vector2f(0.f, 0.f);

      float r1_cross_t = cross(r1, tan);
      float r2_cross_t = cross(r2, tan);
      float f_denom = inv_m1 + inv_m2 + (r1_cross_t * r1_cross_t) * inv_I1 + (r2_cross_t * r2_cross_t) * inv_I2;
      
      float jt = 0.f;
      if (f_denom > 0.f)
        jt = -dot(rel_v, tan) / f_denom;
      
      float mu = std::sqrt(objects[i]->m_mu * objects[j]->m_mu);
      
      if (std::fabs(jt) > _j * mu)
        jt = (jt < 0.f) ? -_j * mu : _j * mu;

      sf::Vector2f f_impulse = jt * tan;
      objects[i]->m_v += f_impulse * inv_m1;
      objects[j]->m_v -= f_impulse * inv_m2;
      objects[i]->m_av += cross(r1, f_impulse) * inv_I1;
      objects[j]->m_av -= cross(r2, f_impulse) * inv_I2;


      const float EPSILON = 1e-6f;

      if (std::fabs(objects[i]->m_av) > EPSILON)
        objects[i]->SetRotationCenter(objects[i]->m_mass_center - perpendicular(objects[i]->m_v) / objects[i]->m_av);
      else
        objects[i]->SetRotationCenter(objects[i]->m_mass_center);

      if (std::fabs(objects[j]->m_av) > EPSILON)
        objects[j]->SetRotationCenter(objects[j]->m_mass_center - perpendicular(objects[j]->m_v) / objects[j]->m_av);
      else
        objects[j]->SetRotationCenter(objects[j]->m_mass_center);
    }

    if (objects[i]->m_in_contact)
      objects[i]->SetRotationCenter(objects[i]->m_mass_center);
  }
}



Engine::Object::Object(const std::vector<sf::Vector2f> &vertices, sf::Vector2f pos) :
  m_enable_collision(false),
  m_enable_movement (false),
  m_enable_gravity  (false),
  m_enable_rotation (false),

  m_v ({ 0.f, 0.f }),
  m_av(0.f),
  m_m (0.f),
  m_e (0.f),
  m_mu(1.f),

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

  update_inertia();
  m_rotation_center = m_mass_center;

  SetPosition(pos);
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
    m_mass_center += s;
    m_rotation_center += s;
    m_sprite.move(s);
  }
  if (m_enable_rotation) {
    float as = m_av * (dt * 1e-9f);
    Rotate(as, m_rotation_center);
  }
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

  if (!m_enable_movement) {
    m_v = { 0.f, 0.f };
    m_m = 0.f;
    m_I = 0.f;
  }
  else if (m_m <= 0.f) {
    m_m = 1.f;
    update_inertia();
  }
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
  if (!m_enable_movement)
    return false;

  if (m <= 0.f)
    return false;

  m_m = m;
  update_inertia();

  return true;
}

bool Engine::Object::SetRestitutionFactor(float e)
{
  if (e < 0.f) {
    m_e = 0.f;
    return false;
  }
  else if (e > 1.f) {
    m_e = 1.f;
    return false;
  }

  m_e = e;
  return true;
}

bool Engine::Object::SetFrictionFactor(float mu)
{
  if (mu < 0.f) {
    m_mu = 0.f;
    return false;
  }
  else if (mu > 1.f) {
    m_mu = 1.f;
    return false;
  }

  m_mu = mu;
  return true;
}

void Engine::Object::SetPosition(sf::Vector2f pos)
{
  auto s = pos - m_center;
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;

  m_mass_center += s;
  m_rotation_center += s;

  m_center = pos;
  m_sprite.setPosition(pos);
}

void Engine::Object::Move(sf::Vector2f s)
{
  for (size_t i = 0; i < m_body.getVertexCount(); ++i)
    m_body[i].position += s;
  
  m_center += s;
  m_mass_center += s;
  m_rotation_center += s;

  m_sprite.move(s);
}

void Engine::Object::Rotate(float rad, sf::Vector2f center)
{
  auto rotate_matrix_multiply = [](sf::Vector2f &point, float angle)
  {
    point = {
      (float)(cos(angle) * point.x -
      sin(angle) * point.y),

      (float)(sin(angle) * point.x +
      cos(angle) * point.y)
    };
  };

  for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
    m_body[i].position -= center;
    rotate_matrix_multiply(m_body[i].position, rad);
    m_body[i].position += center;
  }

  m_center -= center;
  rotate_matrix_multiply(m_center, rad);
  m_center += center;

  m_mass_center -= center;
  rotate_matrix_multiply(m_mass_center, rad);
  m_mass_center += center;

  m_rotation_center -= center;
  rotate_matrix_multiply(m_rotation_center, rad);
  m_rotation_center += center;

  auto sprite_origin = m_sprite.getOrigin();
  m_sprite.setOrigin(center);
  m_sprite.rotate(rad);
  m_sprite.setOrigin(sprite_origin);
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

void Engine::Object::SetRotationCenter(sf::Vector2f pos)
{ 
  if (m_rotation_center == pos)
    return;

  m_rotation_center = pos;
  update_inertia();
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

std::vector<sf::Vector2f> Engine::Object::GetIntersection(const Object &object) const
{
  static const auto is_inside = [](
    const sf::Vector2f &p, const sf::Vector2f &A, const sf::Vector2f &B
  )
  {
    return cross(B - A, p - A) >= 0;
  };
  static const auto compute_intersection = [](
    const sf::Vector2f &P, const sf::Vector2f &Q,
    const sf::Vector2f &A, const sf::Vector2f &B
  )
  {
    sf::Vector2f PQ = Q - P;
    sf::Vector2f AB = B - A;
    float t = cross(A - P, AB) / cross(PQ, AB);

    return P + t * PQ;
  };


  std::vector<sf::Vector2f> out(m_body.getVertexCount());
  for (size_t i = 0; i < out.size(); ++i)
    out[i] = m_body[i].position;

  size_t clip_count = object.m_body.getVertexCount();
  for (size_t i = 0; i < clip_count; ++i) {
    std::vector<sf::Vector2f> in = out;
    out.clear();
    sf::Vector2f A = object.m_body[i].position;
    sf::Vector2f B = object.m_body[(i + 1) % clip_count].position;
    if (in.empty())
      break;
    for (size_t j = 0; j < in.size(); ++j) {
      sf::Vector2f P = in[j];
      sf::Vector2f Q = in[(j + 1) % in.size()];
      bool P_inside = is_inside(P, A, B);
      bool Q_inside = is_inside(Q, A, B);
      if (P_inside && Q_inside)
        out.push_back(Q);
      else if (P_inside && !Q_inside)
        out.push_back(compute_intersection(P, Q, A, B));
      else if (!P_inside && Q_inside) {
        out.push_back(compute_intersection(P, Q, A, B));
        out.push_back(Q);
      }
    }
  }
  return out;
}

bool Engine::Object::Contains(const sf::Vector2f &point) const
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



Engine::Object::CollisionInfo Engine::Object::get_collision_info(
  const Object &object
) const
{
  float        min_overlap     = std::numeric_limits<float>::infinity();
  sf::Vector2f best_normal;
  bool         collision_found = true;

  static const auto get_edges =
  [](const sf::VertexArray &poly) -> std::vector<sf::Vector2f> 
  {
    std::vector<sf::Vector2f> edges;
    for (size_t i = 0; i < poly.getVertexCount(); ++i) {
      sf::Vector2f v1 = poly[i].position;
      sf::Vector2f v2 = poly[(i + 1) % poly.getVertexCount()].position;

      edges.push_back(v2 - v1);
    }
    return edges;
  };

  static const auto project_polygon =
  [](const sf::VertexArray &poly, sf::Vector2f axis) -> std::pair<float, float> 
  {
    float min_proj = std::numeric_limits<float>::infinity();
    float max_proj = -std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < poly.getVertexCount(); ++i) {
      float proj = dot(poly[i].position, axis);
      min_proj = std::min(min_proj, proj);
      max_proj = std::max(max_proj, proj);
    }
    return { min_proj, max_proj };
  };
  

  auto edges1 = get_edges(m_body);
  auto edges2 = get_edges(object.m_body);
  
  for (auto &edge : edges1) {
    sf::Vector2f axis = normalize(perpendicular(edge));
    auto [min1, max1] = project_polygon(m_body, axis);
    auto [min2, max2] = project_polygon(object.m_body, axis);
    float overlap = std::min(max1, max2) - std::max(min1, min2);
    if (overlap < 0.f) {
      collision_found = false;
      break;
    }
    if (overlap < min_overlap) {
      min_overlap = overlap;
      best_normal = axis;
    }
  }

  for (const auto &edge : edges2) {
    sf::Vector2f axis = normalize(perpendicular(edge));
    auto [min1, max1] = project_polygon(m_body, axis);
    auto [min2, max2] = project_polygon(object.m_body, axis);
    float overlap = std::min(max1, max2) - std::max(min1, min2);
    if (overlap < 0.f) {
      collision_found = false;
      break;
    }
    if (overlap < min_overlap) {
      min_overlap = overlap;
      best_normal = axis;
    }
  }

  if (!collision_found) {
    return { false, sf::Vector2f(0.f, 0.f), 0.f, {} };
  }


  sf::Vector2f direction = GetPosition() - object.GetPosition();
  if (dot(best_normal, direction) < 0)
    best_normal = -best_normal;

  auto contact_points = GetIntersection(object);
  if (contact_points.empty()) {
    for (size_t i = 0; i < m_body.getVertexCount(); ++i) {
      sf::Vector2f pt = m_body[i].position;
      if (object.Contains(pt))
        contact_points.push_back(pt);
    }
  }
  if (contact_points.empty()) {
    for (size_t i = 0; i < object.m_body.getVertexCount(); ++i) {
      sf::Vector2f pt = object.m_body[i].position;
      if (Contains(pt))
        contact_points.push_back(pt);
    }
  }
  if (contact_points.empty()) {
    return { false, sf::Vector2f(0.f, 0.f), 0.f, {} };
  }

  return { true, best_normal, min_overlap, contact_points };
}

void Engine::Object::update_inertia()
{
  if (m_body.getVertexCount() < 3)
    return;
  
  size_t n = m_body.getVertexCount();
  float area = 0.f;
  m_mass_center = { 0.f, 0.f };
  
  for (size_t i = 0; i < n; i++) {
    size_t j = (i + 1) % n;
    float cross_val = cross(m_body[i].position, m_body[j].position);
    area += cross_val;
    m_mass_center += (m_body[i].position + m_body[j].position) * cross_val;
  }
  area *= 0.5f;
  
  float abs_area = std::fabs(area);
  m_mass_center /= (6.f * area);
  
  std::vector<sf::Vector2f> centered_vertices;
  centered_vertices.reserve(n);
  for (size_t i = 0; i < n; i++) {
    centered_vertices.push_back(m_body[i].position - m_mass_center);
  }

  float I_sum = 0.f;
  for (size_t i = 0; i < n; i++) {
    size_t j = (i + 1) % n;
    float crossVal = cross(centered_vertices[i], centered_vertices[j]);
    float term = (
      dot(centered_vertices[i], centered_vertices[i]) +
      dot(centered_vertices[i], centered_vertices[j]) +
      dot(centered_vertices[j], centered_vertices[j])
    );
    I_sum += crossVal * term;
  }

  m_I = std::fabs((m_m / (6.f * abs_area)) * I_sum);
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

