#ifndef ENGINE_MAP_HPP
#define ENGINE_MAP_HPP


// This class is for creating maps and loading them from files,
// not for play time


#include "engine.hpp"
#include "objects/objects.hpp"


class Engine::Map
{
public:

  Map(
    sf::Vector2f        size,
    sf::Texture        &bg,
    std::vector<Object> objects,
    sf::Vector2f        spawn_point
  );
  Map(
    const std::string &path
  );

  ~Map() = default;

private:
};


#endif // !ENGINE_MAP_HPP
