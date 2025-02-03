#ifndef MISC_ANIMATED_SPRITE_HPP
#define MISC_ANIMATED_SPRITE_HPP


#include <SFML/Graphics.hpp>

#include "misc/misc.hpp"


class AnimatedSprite : public sf::Sprite
{
public:

  struct Frame
  {
    sf::IntRect  rect;
    float        duration;  // milliseconds
    sf::Texture *texture;
  };

  
  bool Pause;


  AnimatedSprite();
  AnimatedSprite(const sf::Texture &texture, std::vector<Frame> frames={});
  AnimatedSprite(TextureAtlas texture_atlas);

  ~AnimatedSprite() = default;


  void Update(uint64_t dt);

  void AddFrame   (const Frame              &frame);
  void AddFrames  (const std::vector<Frame> &frames);
  void ClearFrames();

  void SwitchAnimation(TextureAtlas texture_atlas);

  void ResetTime();


  // AnimatedSprite(const AnimatedSprite&) = delete;

private:

  std::vector<Frame> m_frames;
  uint64_t           m_cur_frame;
  float              m_frame_time;

};


#endif // !MISC_ANIMATED_SPRITE_HPP
