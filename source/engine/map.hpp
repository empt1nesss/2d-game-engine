#ifndef ENGINE_MAP_HPP
#define ENGINE_MAP_HPP


// This sruct is for creating maps and loading them from files,
// not for play time


#include "engine.hpp"
#include "objects/objects.hpp"

#include <set>


struct Engine::Map
{
public:

  struct BgObject
  {
    mutable sf::Sprite SfSprite;

    BgObject(sf::Texture &text, float depth) :
      SfSprite(text), m_depth(depth)
    {}

    void SetDepth(float depth)
    {
      if (depth > 1.f)
        m_depth = 1.f;
      else if (depth < 0.f)
        m_depth = 0.f;
      else
        m_depth = depth;
    };
    float GetDepth() const { return m_depth; }


    bool operator<(const BgObject &bg_obj) const
    {
      return m_depth < bg_obj.m_depth;
    }

  private:

    float m_depth;

  };


  Map() = default;
  Map(
    const std::string &path
  );

  sf::Vector2f            Size;
  std::multiset<BgObject> BgObjects;
  std::vector<Object>     Objects;
  sf::Vector2f            Spawnpoint;

};


#endif // !ENGINE_MAP_HPP
