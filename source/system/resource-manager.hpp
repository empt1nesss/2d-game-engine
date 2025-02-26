#ifndef SYSTEM_RESOURCE_MANAGER_HPP
#define SYSTEM_RESOURCE_MANAGER_HPP


#include <SFML/Graphics.hpp>


class ResourceManager
{
public:
  
  enum Err
  {
    NOT_FOUND_EXC
  };


  bool         LoadTexture(const std::string &name, const std::string fileformat="png");

  const sf::Texture* GetTexture(const std::string &name) const;
  std::string        GetMapPath(const std::string &name) const;
  
private:

  sf::Texture                        m_default_texture;
  std::map<std::string, sf::Texture> m_textures;

};


#endif // !SYSTEM_RESOURCE_MANAGER_HPP

