#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "engine/engine.hpp"
#include "misc/animated-sprite.hpp"

#include <cstdint>
#include <vector>


class Engine::Object
{
public:

  static void ResolveCollision(const std::vector<Object*> &objects);


  Object(
    const std::vector<sf::Vector2f> &vertices,
    sf::Vector2f                     pos     ={ 0.f, 0.f }
  );
  virtual ~Object() = default;


  virtual void Update(uint64_t          dt);
  virtual void Render(sf::RenderTarget &target);


  int  ZIndex;
  bool DrawBody;


  void EnableCollision(bool state);
  void EnableMovement (bool state);
  void EnableGravity  (bool state);
  void EnableRotation (bool state);

  bool IsCollisionEnabled() const { return m_enable_collision; }
  bool IsMovementEnabled () const { return m_enable_movement; }
  bool IsGravityEnabled  () const { return m_enable_gravity; };
  bool IsRotationEnabled () const { return m_enable_rotation; }

  bool SetSpeed            (const sf::Vector2f &v);
  bool SetAngularSpeed     (float               av);
  bool SetMass             (float               m);
  bool SetRestitutionFactor(float               e);
  bool SetFrictionFactor   (float               mu);

  sf::Vector2f GetSpeed            () const { return m_v; }
  float        GetAngularSpeed     () const { return m_av; }
  float        GetMass             () const { return m_m; }
  float        GetRestitutionFactor() const { return m_e; }
  float        GetFrictionFactor   () const { return m_mu; }

  void SetPosition      (sf::Vector2f pos);
  void Move             (sf::Vector2f s);
  void Rotate           (float        rad,          sf::Vector2f center);
  void SetTexture       (TextureAtlas texture_atlas);
  void SetSpriteScale   (sf::Vector2f scale);
  void SetSpriteOrigin  (sf::Vector2f origin);
  void SetSpriteRotation(float        angle);
  void SetBodyColor     (sf::Color    color);

  void SetRotationCenter(sf::Vector2f pos);

  const AnimatedSprite&  GetSprite  () const { return m_sprite; }
  sf::Vector2f           GetPosition() const { return m_center; }
  const sf::VertexArray& GetBody    () const { return m_body; }

  bool                      Contains       (const sf::Vector2f &point)  const;
  std::vector<sf::Vector2f> GetIntersection(const Object       &object) const;

  bool InContact() const { return m_in_contact; }
  bool OnGround () const { return m_on_ground; }

protected:

  sf::VertexArray m_body;
  AnimatedSprite  m_sprite;

private:

  struct CollisionInfo {
    bool                      collision_detected;
    sf::Vector2f              normal;
    float                     depth;
    std::vector<sf::Vector2f> contact_points;
  };


  sf::Vector2f    m_center;
  sf::Vector2f    m_mass_center;
  sf::Vector2f    m_rotation_center;

  sf::Vector2f m_v;
  float        m_av;
  float        m_m;
  float        m_e;
  float        m_I;
  float        m_mu;

  bool m_in_contact;
  bool m_on_ground;

  bool m_enable_collision;
  bool m_enable_movement;
  bool m_enable_gravity;
  bool m_enable_rotation;


  CollisionInfo get_collision_info(const Object &object) const;
  void          update_inertia    ();

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


class Engine::Light : Engine::Object
{
public:
  
  Light(float radius, sf::Color color, sf::Vector2f pos={ 0.f, 0.f }, unsigned vertix = 24);

  void SetColor           (sf::Color    color);
  void SetRadius          (float        radius);
  void SetRadius          (float        radius, unsigned vertex_count);
  void SetAngle           (float        angle);
  void SetPosition        (sf::Vector2f pos);
  void SetRotation        (float        angle);
  void SetBrightnessLevel (uint8_t brightness_level);

  sf::Color    GetColor   () const {return m_color;    };
  float        GetRadius  () const {return m_radius;   };
  float        GetAngle   () const {return m_angle;    };
  sf::Vector2f GetPosition() const {return m_pos;      }; 
  float        GetRotation() const {return m_rotation; };

  Object&             Object()       { return *this; }
  const class Object& Object() const { return *this; }

private:

  void create_gradient();
  void add_center     ();

  sf::Color    m_color;
  float        m_radius;
  float        m_angle;
  sf::Vector2f m_pos;
  float        m_rotation;
  unsigned     m_vertex_count;
  uint8_t      m_brightness_level;

};



#endif // !GAME_OBJECTS_HPP

