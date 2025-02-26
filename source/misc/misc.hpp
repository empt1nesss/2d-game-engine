#ifndef MISC_MISC_HPP
#define MISC_MISC_HPP


#include <SFML/Graphics.hpp>
#include <json.hpp>


struct TextureAtlas
{
  std::string  texture_alias;
  unsigned     frame_width   = 0;
  unsigned     frame_height  = 0;
  unsigned     rows          = 0;
  unsigned     cols          = 0;
  unsigned     offset_x      = 0;
  unsigned     offset_y      = 0;
};


float        dot          (const sf::Vector2f &v1, const sf::Vector2f &v2);
float        cross        (const sf::Vector2f &v1, const sf::Vector2f &v2);
sf::Vector2f normalize    (const sf::Vector2f &v);
sf::Vector2f perpendicular(const sf::Vector2f &v);
float        length       (const sf::Vector2f &v);
float        deg          (float               rad);
float        rad          (float               deg);

Json::Value serialize_vector(const sf::Vector2f &v);
Json::Value serialize_color (const sf::Color &c);


#endif // !MISC_MISC_HPP
