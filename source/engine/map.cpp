#include "map.hpp"
#include "objects/objects.hpp"


Json::StructType Engine::Map::Serialize() const
{
  return {
    { "size",       serialize_vector(Size) },
    { "spawnpoint", serialize_vector(Spawnpoint) },
    {
      "objects",
      Json::Value(
        Objects.begin(), Objects.end(),
        std::function<Json::Value(const Object&)>(
          [](const Object &obj)
          {
            return obj.Serialize();
          }
        )
      )
    },
    {
      "bg-objects",
      Json::Value(
        BgObjects.begin(), BgObjects.end(),
        std::function<Json::Value(const BgObject&)>(
          [](const BgObject &obj)
          {
            return obj.Serialize();
          }
        )
      )
    }
  };
}

