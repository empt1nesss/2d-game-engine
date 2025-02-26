#include "resource-manager.hpp"


bool ResourceManager::LoadTexture(const std::string &name, const std::string fileformat)
{
  sf::Texture player_text;
  if (!player_text.loadFromFile("assets/" + name + "." + fileformat))
    return false;

  m_textures.emplace(name, std::move(player_text));

  return true;
}

sf::Texture* ResourceManager::GetTexture(const std::string &name) const
{
  auto texture = m_textures.find(name);
  if (texture == m_textures.end()) {
    return &m_textures.emplace(
      name, std::move(sf::Texture())
    ).first->second;
  }

  return &texture->second;
}

