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

  sf::Vector2f                  Size;
  sf::Vector2f                  Spawnpoint;
  std::vector<Object>           Objects;
  std::vector<BgObject>         BgObjects;
  std::map<std::string, Object> RuntimeObjects;


  Map() = default;

  Json::StructType Serialize() const;

  Map& Load(
    const std::string     &map_alias,
    const ResourceManager &rm
  );

};


#endif // !ENGINE_MAP_HPP
