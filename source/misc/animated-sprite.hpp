#ifndef MISC_ANIMATED_SPRITE_HPP
#define MISC_ANIMATED_SPRITE_HPP


#include <SFML/Graphics.hpp>


struct Animation
{
  sf::Texture *texture;
  unsigned     frame_width;
  unsigned     frame_height;
  float        frame_time;
  unsigned     rows;
  unsigned     cols;
};


class AnimatedSprite : public sf::Sprite
{
public:

  struct Frame
  {
    sf::IntRect rect;
    float       duration;  // milliseconds
  };


  AnimatedSprite(const sf::Texture &texture, std::vector<Frame> frames={});
  AnimatedSprite(Animation animation);

  ~AnimatedSprite() = default;


  void Update(uint64_t dt);

  void AddFrame   (const Frame              &frame);
  void AddFrames  (const std::vector<Frame> &frames);
  void ClearFrames();

  void SwitchAnimation(Animation animation);

  void ResetTime();


  // AnimatedSprite(const AnimatedSprite&) = delete;

private:

  std::vector<Frame> m_frames;
  uint64_t           m_cur_frame;
  float              m_frame_time;

};


#endif // !MISC_ANIMATED_SPRITE_HPP
