#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "engine/engine.hpp"
#include "misc/animated-sprite.hpp"

#include <vector>


class Engine::Object
{
public:

  static void ResolveCollision(const std::vector<Object*> objects);


  Object(
    const std::vector<sf::Vector2f> &vertices,
    sf::Vector2f                     pos     ={ 0.f, 0.f }
  );


  virtual void Update(uint64_t          dt);
  virtual void Render(sf::RenderTarget &target);


  bool DrawBody;


  void EnableCollision(bool state);
  void EnableMovement (bool state);
  void EnableGravity  (bool state);
  void EnableRotation (bool state);

  bool IsCollisionEnabled() const { return m_enable_collision; }
  bool IsMovementEnabled () const { return m_enable_movement; }
  bool IsGravityEnabled  () const { return m_enable_gravity; };
  bool IsRotationEnabled () const { return m_enable_rotation; }

  bool SetSpeed       (const sf::Vector2f &v);
  bool SetAngularSpeed(float               av);
  bool SetMass        (float               m);
  bool SetRestitution (float               val);

  sf::Vector2f GetSpeed       () const { return m_v; }
  float        GetAngularSpeed() const { return m_av; }
  float        GetMass        () const { return m_m; }
  float        GetRestitution () const { return m_restitution; }

  void SetPosition      (sf::Vector2f pos);
  void Move             (sf::Vector2f s);
  void SetTexture       (TextureAtlas texture_atlas);
  void SetSpriteScale   (sf::Vector2f scale);
  void SetSpriteOrigin  (sf::Vector2f origin);
  void SetSpriteRotation(float        angle);
  void SetBodyColor     (sf::Color    color);

  const AnimatedSprite&  GetSprite  () const { return m_sprite; }
  sf::Vector2f           GetPosition() const { return m_center; }
  const sf::VertexArray& GetBody    () const { return m_body; }

  bool OnGround() const { return m_on_ground; }

  bool IsIntersects(const Object       &object) const;
  bool IsContains  (const sf::Vector2f &point)  const;


  bool operator==(const Object &object) const { return this == &object; }

private:

  struct CollisionInfo {
    bool         collision_detected;
    sf::Vector2f normal;
    float        depth;
  };


  sf::VertexArray m_body;
  AnimatedSprite  m_sprite;
  sf::Vector2f    m_center;

  sf::Vector2f m_v;
  float        m_av;
  float        m_m;
  float        m_restitution;

  bool m_on_ground;

  bool m_enable_collision;
  bool m_enable_movement;
  bool m_enable_gravity;
  bool m_enable_rotation;


  bool          is_on_ground      ();
  CollisionInfo get_collision_info(const Object &object) const;

};


class Engine::RectObject : public Engine::Object
{
public:

  RectObject(sf::Vector2f size, sf::Vector2f pos={ 0.f, 0.f });

};


class Engine::CircleObject : public Engine::Object
{
public:

  CircleObject(
    float        radius,
    sf::Vector2f pos         ={ 0.f, 0.f },
    unsigned     vertex_count=12
  );

private:

  static std::vector<sf::Vector2f> create_circle(
    float    radius,
    unsigned vertex_count
  );

};


#endif // !GAME_OBJECTS_HPP

