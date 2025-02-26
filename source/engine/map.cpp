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

Engine::Map& Engine::Map::Load(
  const std::string     &map_alias,
  const ResourceManager &rm
)
{
  Json j;
  j.FromFile(rm.GetMapPath(map_alias));
  
  Size       = deserialize_vector(j.GetData()["size"]);
  Spawnpoint = deserialize_vector(j.GetData()["spawnpoint"]);
  
  for (auto &obj : j.GetData()["objects"].GetList()) {
    Objects.emplace_back(std::move(Object(obj, rm)));
  }
  
  for (auto &bg_obj : j.GetData()["bg-objects"].GetList()) {
    BgObjects.emplace_back(std::move(BgObject(bg_obj, rm)));
  }

  return *this;
}

