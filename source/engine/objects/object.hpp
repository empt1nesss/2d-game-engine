#ifndef ENGINE_OBJECTS_OBJECT_HPP
#define ENGINE_OBJECTS_OBJECT_HPP


#include <json.hpp>

#include "engine/engine.hpp"
#include "misc/animated-sprite.hpp"

#include <vector>


class Engine::Object
{
public:

  static Object CreateRectObj(
    sf::Vector2f size,
    sf::Vector2f pos={ 0.f, 0.f }
  );
  static Object CreateCircleObj(
    float        radius,
    sf::Vector2f pos         ={ 0.f, 0.f },
    unsigned     vertex_count=12
  );

  static void ResolveCollision(const std::vector<Object*> &objects);


  Object(
    const std::vector<sf::Vector2f> &vertices,
    sf::Vector2f                     pos     ={ 0.f, 0.f }
  );
  explicit Object(
    const Json::Value     &val,
    const ResourceManager &rm
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
  void SetBodyColor     (sf::Color    color);

  void SetRotationCenter(sf::Vector2f pos);

  AnimatedSprite&        GetSprite  ()       { return m_sprite; }
  const AnimatedSprite&  GetSprite  () const { return m_sprite; }
  sf::Vector2f           GetPosition() const { return m_center; }
  const sf::VertexArray& GetBody    () const { return m_body; }

  bool                      Contains       (const sf::Vector2f &point)  const;
  std::vector<sf::Vector2f> GetIntersection(const Object       &object) const;

  bool InContact() const { return m_in_contact; }
  bool OnGround () const { return m_on_ground; }

  Json::Value Serialize() const;

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

  Json::ListType serialize_body() const;

};


#endif // !ENGINE_OBJECTS_OBJECT_HPP

