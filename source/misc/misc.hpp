#ifndef MISC_MISC_HPP
#define MISC_MISC_HPP


#include <SFML/Graphics.hpp>


struct TextureAtlas
{
  sf::Texture *texture;
  unsigned     frame_width;
  unsigned     frame_height;
  float        frame_time;
  unsigned     rows;
  unsigned     cols;
  unsigned     offset_x     = 0;
  unsigned     offset_y     = 0;
};


float        dot          (const sf::Vector2f &v1, const sf::Vector2f &v2);
float        cross        (const sf::Vector2f &v1, const sf::Vector2f &v2);
sf::Vector2f normalize    (const sf::Vector2f &v);
sf::Vector2f perpendicular(const sf::Vector2f &v);
float        length       (const sf::Vector2f &v);
float        deg          (float               rad);
float        rad          (float               deg);


#endif // !MISC_MISC_HPP
