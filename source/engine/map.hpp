#ifndef ENGINE_MAP_HPP
#define ENGINE_MAP_HPP


// This sruct is for creating maps and loading them from files,
// not for play time


#include "engine.hpp"
#include "objects/objects.hpp"


struct Engine::Map
{
public:

  Map() = default;
  Map(
    const std::string &path
  );

  sf::Vector2f        Size;
  sf::Texture         BgTexture;
  // std::vector<Object> Objects;
  sf::Vector2f        Spawnpoint;

};


#endif // !ENGINE_MAP_HPP
