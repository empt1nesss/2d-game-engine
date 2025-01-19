#include "animated-sprite.hpp"


AnimatedSprite::AnimatedSprite(const sf::Texture &texture, std::vector<Frame> frames) :
  sf::Sprite(texture), m_frames(frames), m_cur_frame(0), m_frame_time(0.f)
{
  if (!m_frames.empty()) {
    Sprite::setTextureRect(m_frames[0].rect);
  }
}

AnimatedSprite::AnimatedSprite(Animation animation) :
  m_frames(animation.rows * animation.cols)
{
  SwitchAnimation(animation);
}


void AnimatedSprite::Update(uint64_t dt)
{
  if (m_frames.empty())
    return;

  m_frame_time += (dt * 1e-6);
  while (m_frame_time >= m_frames[m_cur_frame].duration) {
    m_frame_time -= m_frames[m_cur_frame].duration;

    ++m_cur_frame;
    if (m_cur_frame == m_frames.size())
      m_cur_frame = 0;

    Sprite::setTextureRect(m_frames[m_cur_frame].rect);
  }
}

void AnimatedSprite::AddFrame(const Frame &frame)
{
  m_frames.push_back(frame);
  if (m_frames.size() == 1) {
    Sprite::setTextureRect(m_frames[0].rect);
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
  }
}

void AnimatedSprite::ClearFrames()
{
  m_frames.clear();
  ResetTime();
}

void AnimatedSprite::SwitchAnimation(Animation animation)
{
  sf::Sprite::setTexture(*animation.texture);
  for (unsigned row = 0; row < animation.rows; ++row) {
    for (unsigned col = 0; col < animation.cols; ++col) {
      m_frames[row * animation.cols + col] = {
        {
          (int)(animation.frame_width * col),
          (int)(animation.frame_height * row),
          (int)animation.frame_width,
          (int)animation.frame_height
        },
        animation.frame_time
      };
    } 
  }
  ResetTime();
}

void AnimatedSprite::ResetTime()
{
  m_frame_time = 0.f;
  m_cur_frame  = 0;
}
