#ifndef MISC_ANIMATED_SPRITE_HPP
#define MISC_ANIMATED_SPRITE_HPP


#include <SFML/Graphics.hpp>
#include <json.hpp>

#include "misc/misc.hpp"
#include "system/resource-manager.hpp"


class AnimatedSprite : public sf::Sprite
{
public:
  
  bool Pause;


  AnimatedSprite();
  AnimatedSprite(
    TextureAtlas           texture_atlas,
    const ResourceManager &rm,
    float                  frame_time   =0.f
  );
  AnimatedSprite(
    const Json::Value     &val,
    const ResourceManager &rm
  );

  ~AnimatedSprite() = default;


  void Update         (uint64_t dt);
  void SwitchAnimation(
    TextureAtlas texture_atlas, const ResourceManager &rm
  );

  void ResetTime();

  float GetFrameTime() const { return m_frame_time; }

  bool SetFrameTime(float t)
  {
    if (t < 0.f)
      return false;

    m_frame_time = t;
    return true;
  }

  void setOrigin(const sf::Vector2f &val);
  void rotate   (float angle);

  Json::StructType Serialize() const;

private:

  TextureAtlas             m_texture_atlas;
  std::vector<sf::IntRect> m_frames;
  sf::IntRect              m_atlas_rect;
  uint64_t                 m_cur_frame;
  float                    m_frame_time;
  float                    m_time;
  sf::Vector2f             m_origin;
  float                    m_rotation;

};


#endif // !MISC_ANIMATED_SPRITE_HPP
