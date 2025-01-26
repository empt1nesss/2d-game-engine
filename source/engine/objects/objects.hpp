#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "engine/engine.hpp"


class Engine::Object
{
public:

  virtual void Update(uint64_t          dt)     = 0;
  virtual void Render(sf::RenderTarget &target) = 0;

};


#endif // !GAME_OBJECTS_HPP
