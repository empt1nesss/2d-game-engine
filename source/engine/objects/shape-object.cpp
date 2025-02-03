#include "objects.hpp"

#include <cmath>


Engine::ShapeObject::ShapeObject()
{
  Vertices.setPrimitiveType(sf::TriangleFan);
}


void Engine::ShapeObject::Update(uint64_t dt)
{
  if (EnableGravity && EnableMovement) {
    Speed.y += (dt * 1e-9f * GravityAcceleration);
  }
  
  if (EnableMovement && EnableRotation)
  {
    sf::Vector2f s      = Speed * (dt * 1e-9f);
    float        as     = AngularSpeed * (dt * 1e-9f);
    sf::Vector2f centre;

    for (size_t i = 0; i < Vertices.getVertexCount(); ++i) {
      Vertices[i].position += s;
      centre += Vertices[i].position;
    }

    centre /= (float)Vertices.getVertexCount();

    for (size_t i = 0; i < Vertices.getVertexCount(); ++i) {
      Vertices[i].position -= centre;

      Vertices[i].position = {
        (float)(cos(as) * Vertices[i].position.x +
        -sin(as) * Vertices[i].position.y),

        (float)(sin(as) * Vertices[i].position.x +
        cos(as) * Vertices[i].position.y)
      };

      Vertices[i].position += centre;
    }
  }
  else if (EnableMovement) {
    sf::Vector2f s = Speed * (dt * 1e-9f);
    for (size_t i = 0; i < Vertices.getVertexCount(); ++i) {
      Vertices[i].position += s;
    }
  }
  else if (EnableRotation) {
    sf::Vector2f centre;
    float        as     = AngularSpeed * (dt * 1e-9f);

    for (size_t i = 0; i < Vertices.getVertexCount(); ++i) {
      centre += Vertices[i].position;
    }

    centre /= (float)Vertices.getVertexCount();

    for (size_t i = 0; i < Vertices.getVertexCount(); ++i) {
      Vertices[i].position -= centre;

      Vertices[i].position = {
        (float)(cos(as) * Vertices[i].position.x +
        -sin(as) * Vertices[i].position.y),

        (float)(sin(as) * Vertices[i].position.x +
        cos(as) * Vertices[i].position.y)
      };

      Vertices[i].position += centre;
    }
  }

  if (EnableCollision) {
    // to do
  }

}

void Engine::ShapeObject::Render(sf::RenderTarget &target)
{
  target.draw(Vertices);
}


