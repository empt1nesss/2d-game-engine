#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "engine/engine.hpp"
#include "misc/animated-sprite.hpp"

#include <vector>


class Engine::Object
{
public:

  Object(
    const std::vector<sf::Vector2f> &vertices,
    sf::Vector2f                     pos     ={ 0.f, 0.f }
  );


  virtual void Update(uint64_t          dt);
  virtual void Render(sf::RenderTarget &target);


  bool EnableCollision;
  bool EnableMovement;
  bool EnableGravity;
  bool EnableRotation;
  
  sf::Vector2f Speed;
  float        AngularSpeed;
  float        Mass;

  bool DrawBody;
  

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

  bool IsFalling() const; // to do


  bool operator==(const Object &object) const { return this == &object; }

protected:

  static std::vector<Object*> m_objects;

private:

  sf::VertexArray m_body;
  AnimatedSprite  m_sprite;
  sf::Vector2f    m_center;

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
    uint         vertex_count=12
  );

private:

  static std::vector<sf::Vector2f> create_circle(
    float radius,
    uint  vertex_count
  );

};


#endif // !GAME_OBJECTS_HPP

