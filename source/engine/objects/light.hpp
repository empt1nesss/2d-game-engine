#ifndef ENGINE_OBJECTS_LIGHT_HPP
#define ENGINE_OBJECTS_LIGHT_HPP


#include "object.hpp"


class Engine::Light : Engine::Object
{
public:
  
  Light(float radius, sf::Color color, sf::Vector2f pos={ 0.f, 0.f }, unsigned vertix = 24);
  Light(const Json::Value &val);

  void SetColor   (sf::Color    color);
  void SetRadius  (float        radius);
  void SetRadius  (float        radius, unsigned vertex_count);
  void SetAngle   (float        angle);
  void SetPosition(sf::Vector2f pos);
  void SetRotation(float        angle);
  void SetAlpha   (uint8_t a);

  sf::Color    GetColor   () const { return m_color;    };
  float        GetRadius  () const { return m_radius;   };
  float        GetAngle   () const { return m_angle;    };
  sf::Vector2f GetPosition() const { return m_pos;      }; 
  float        GetRotation() const { return m_rotation; };

  Object&             Object()       { return *this; }
  const class Object& Object() const { return *this; }

  Json::Value Serialize() const;

private:

  void create_gradient();
  void add_center     ();

  sf::Color    m_color;
  float        m_radius;
  float        m_angle;
  sf::Vector2f m_pos;
  float        m_rotation;
  unsigned     m_vertex_count;

};


#endif // !ENGINE_OBJECTS_LIGHT_HPP

