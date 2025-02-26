#ifndef ENGINE_OBJECTS_BG_OBJECT_HPP
#define ENGINE_OBJECTS_BG_OBJECT_HPP


#include "object.hpp"


class Engine::BgObject : Engine::Object
{
public:

  BgObject(
    TextureAtlas     texture_atlas,
    ResourceManager &rm,
    float            depth         =0.f,
    sf::Vector2f     pos           ={ 0.f, 0.f }
  );
  

  void Update(uint64_t          dt);
  void Render(sf::RenderTarget &target);

  float GetDepth() const { return m_depth; }

  void SetDepth(float depth);

  sf::Vector2f GetPosition    () const { return Object::GetPosition(); }
  sf::Vector2f GetSpeed       () const { return Object::GetSpeed(); }
  float        GetAngularSpeed() const { return Object::GetAngularSpeed(); }

  void SetPosition    (const sf::Vector2f &pos) { Object::SetPosition(pos); }
  void SetSpeed       (const sf::Vector2f &v)   { Object::SetSpeed(v); }
  void SetAngularSpeed(float               av)  { Object::SetAngularSpeed(av); }

  AnimatedSprite&       GetSprite()       { return Object::GetSprite(); }
  const AnimatedSprite& GetSprite() const { return Object::GetSprite(); }

  Json::StructType Serialize() const;

private:

  float m_depth;

};



#endif // !ENGINE_OBJECTS_BG_OBJECT_HPP

