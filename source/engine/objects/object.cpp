#include "objects.hpp"


std::vector<Engine::Object*> Engine::Object::m_objects;


Engine::Object::Object() :
  EnableCollision(false),
  EnableMovement (false),
  EnableGravity  (false),
  EnableRotation (false),

  Speed       ({ 0.f, 0.f }),
  AngularSpeed(0.f),
  Mass        (0.f)
{
  m_objects.push_back(this);
}

