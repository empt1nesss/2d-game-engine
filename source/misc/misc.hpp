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


#endif // !MISC_MISC_HPP
