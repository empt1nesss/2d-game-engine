#include "animated-sprite.hpp"


AnimatedSprite::AnimatedSprite() :
  Pause       (false),
  m_cur_frame (0),
  m_time      (0.f),
  m_rotation  (0.f),
  m_frame_time(0.f)
{}

AnimatedSprite::AnimatedSprite(
  TextureAtlas           texture_atlas,
  const ResourceManager &rm,
  float                  frame_time
) :
  Pause       (false),
  m_frames    (texture_atlas.rows * texture_atlas.cols),
  m_frame_time(frame_time)
{
  SwitchAnimation(texture_atlas, rm);
  Sprite::setTextureRect(m_frames[0]);
}


void AnimatedSprite::Update(uint64_t dt)
{
  if (Pause || m_frame_time == 0.f)
    return;

  if (m_frames.empty())
    return;

  m_time += (dt * 1e-6);
  while (m_time >= m_frame_time) {
    m_time -= m_frame_time;

    ++m_cur_frame;
    if (m_cur_frame == m_frames.size())
      m_cur_frame = 0;

    if (m_frames[m_cur_frame].width != 0 && m_frames[m_cur_frame].height != 0) {
      Sprite::setTextureRect(m_frames[m_cur_frame]);
    }
    else {
      Sprite::setTextureRect(m_atlas_rect);
    }
    Sprite::setOrigin(m_origin);
    Sprite::setRotation(deg(m_rotation));
  }
}

void AnimatedSprite::SwitchAnimation(
  TextureAtlas texture_atlas, const ResourceManager &rm
)
{
  m_texture_alias = texture_atlas.texture_alias;

  m_atlas_rect.width  = texture_atlas.frame_width  * texture_atlas.cols;
  m_atlas_rect.height = texture_atlas.frame_height * texture_atlas.rows;
  m_atlas_rect.left   = texture_atlas.offset_x;
  m_atlas_rect.top    = texture_atlas.offset_y;

  m_frames.resize(texture_atlas.cols * texture_atlas.rows);
  sf::Sprite::setTexture(*rm.GetTexture(texture_atlas.texture_alias));
  m_origin = Sprite::getOrigin();
  for (unsigned row = 0; row < texture_atlas.rows; ++row) {
    for (unsigned col = 0; col < texture_atlas.cols; ++col) {
      m_frames[row * texture_atlas.cols + col] = {
        (int)(texture_atlas.frame_width * col + texture_atlas.offset_x),
        (int)(texture_atlas.frame_height * row + texture_atlas.offset_y),
        (int)texture_atlas.frame_width,
        (int)texture_atlas.frame_height
      };
    } 
  }
  if (!m_frames.empty()) {
    if (m_frames[0].width != 0 && m_frames[0].height != 0) {
      Sprite::setTextureRect(m_frames[0]);
    }
    else {
      Sprite::setTextureRect(m_atlas_rect);
    }
  }

  sf::Sprite::setRotation(deg(m_rotation));

  ResetTime();
}

void AnimatedSprite::ResetTime()
{
  m_time = 0.f;
  m_cur_frame  = 0;
}

void AnimatedSprite::setOrigin(const sf::Vector2f &val)
{
  m_origin = val;
  sf::Sprite::setOrigin(val);
}

void AnimatedSprite::rotate(float angle)
{
  m_rotation += angle;
  sf::Sprite::setRotation(deg(m_rotation));
}

Json::StructType AnimatedSprite::Serialize() const
{
  return {
    { "texture_alias", m_texture_alias },
    { "frame_time",    m_frame_time },
    { "origin",        serialize_vector(m_origin) },
    { "rotation",      m_rotation }
  };
}

