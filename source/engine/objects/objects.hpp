#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "engine/engine.hpp"

#include <vector>


class Engine::Object
{
public:

  virtual void Update(uint64_t          dt)     {};
  virtual void Render(sf::RenderTarget &target) {};


  sf::Vector2f StartPos;

  bool EnableCollision;
  bool EnableMovement;
  bool EnableGravity;
  bool EnableRotation;
  
  sf::Vector2f Speed;
  float        AngularSpeed;
  float        Mass;


  bool operator==(const Object &object) const { return this == &object; }

protected:

  static std::vector<Object*> m_objects;  

  Object();

};


class Engine::SpriteObject : public Engine::Object
{
public:

  SpriteObject();


  virtual void Update(uint64_t          dt)     override;
  virtual void Render(sf::RenderTarget &target) override;

  const sf::Sprite& GetSprite() const { return m_sf_sprite; }

  void SetPosition(sf::Vector2f pos);
  void SetTexture (sf::Texture &texture);

protected:

  sf::Sprite m_sf_sprite;

};


class Engine::ShapeObject : public Engine::Object
{
public:

  sf::VertexArray Vertices;

  
  ShapeObject();


  virtual void Update(uint64_t          dt)     override;
  virtual void Render(sf::RenderTarget &target) override;


};


#endif // !GAME_OBJECTS_HPP
