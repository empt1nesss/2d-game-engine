#ifndef ENGINE_MAP_HPP
#define ENGINE_MAP_HPP


// This sruct is for creating maps and loading them from files,
// not for play time


#include <json.hpp>

#include "engine.hpp"
#include "objects/objects.hpp"

#include <set>


struct Engine::Map
{
public:

  sf::Vector2f          Size;
  std::vector<BgObject> BgObjects;
  std::vector<Object>   Objects;
  sf::Vector2f          Spawnpoint;


  Map() = default;

  Json::StructType Serialize() const;

  Map& Load(
    const std::string     &map_alias,
    const ResourceManager &rm
  );

};


#endif // !ENGINE_MAP_HPP
