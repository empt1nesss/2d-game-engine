#include "animated-sprite.hpp"


AnimatedSprite::AnimatedSprite() :
  Pause       (false),
  m_cur_frame (0),
  m_frame_time(0.f),
  m_rotation  (0.f)
{}

AnimatedSprite::AnimatedSprite(const sf::Texture &texture, std::vector<Frame> frames) :
  sf::Sprite(texture),
  Pause(false),
  m_frames(frames), m_cur_frame(0), m_frame_time(0.f)
{
  if (!m_frames.empty()) {
    Sprite::setTextureRect(m_frames[0].rect);
    m_origin = Sprite::getOrigin();
  }
}

AnimatedSprite::AnimatedSprite(TextureAtlas texture_atlas) :
  Pause(false),
  m_frames(texture_atlas.rows * texture_atlas.cols)
{
  SwitchAnimation(texture_atlas);
  Sprite::setTextureRect(m_frames[0].rect);
}


void AnimatedSprite::Update(uint64_t dt)
{
  if (Pause)
    return;

  if (m_frames.empty())
    return;

  m_frame_time += (dt * 1e-6);
  while (m_frame_time >= m_frames[m_cur_frame].duration) {
    m_frame_time -= m_frames[m_cur_frame].duration;
    auto last_text_ptr = m_frames[m_cur_frame].texture;

    ++m_cur_frame;
    if (m_cur_frame == m_frames.size())
      m_cur_frame = 0;

    if (m_frames[m_cur_frame].texture != last_text_ptr)
      Sprite::setTexture(*m_frames[m_cur_frame].texture);

    Sprite::setTextureRect(m_frames[m_cur_frame].rect);
    Sprite::setOrigin(m_origin);
    Sprite::setRotation(deg(m_rotation));
  }
}

void AnimatedSprite::AddFrame(const Frame &frame)
{
  m_frames.push_back(frame);
  if (m_frames.size() == 1) {
    Sprite::setTextureRect(m_frames[0].rect);
    m_origin = Sprite::getOrigin();
  }
}

void AnimatedSprite::AddFrames(const std::vector<Frame> &frames)
{
  uint64_t i = m_frames.size();
  m_frames.resize(i + frames.size());

  for (uint64_t k = 0; k < frames.size(); ++k) {
    m_frames[k + i] = frames[k];
  }

  if (i == 0 && !frames.empty()) {
    Sprite::setTextureRect(m_frames[0].rect);
    m_origin = Sprite::getOrigin();
  }
}

void AnimatedSprite::ClearFrames()
{
  m_frames.clear();
  ResetTime();
}

void AnimatedSprite::SwitchAnimation(TextureAtlas texture_atlas)
{
  m_frames.resize(texture_atlas.cols * texture_atlas.rows);
  sf::Sprite::setTexture(*texture_atlas.texture);
  m_origin = Sprite::getOrigin();
  for (unsigned row = 0; row < texture_atlas.rows; ++row) {
    for (unsigned col = 0; col < texture_atlas.cols; ++col) {
      m_frames[row * texture_atlas.cols + col] = {
        {
          (int)(texture_atlas.frame_width * col + texture_atlas.offset_x),
          (int)(texture_atlas.frame_height * row + texture_atlas.offset_y),
          (int)texture_atlas.frame_width,
          (int)texture_atlas.frame_height
        },
        texture_atlas.frame_time,
        texture_atlas.texture
      };
    } 
  }
  if (!m_frames.empty()) {
    Sprite::setTextureRect(m_frames[0].rect);
  }

  sf::Sprite::setRotation(deg(m_rotation));

  ResetTime();
}

void AnimatedSprite::ResetTime()
{
  m_frame_time = 0.f;
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
