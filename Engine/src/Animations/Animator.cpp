#include "Animator.hpp"

Animator::Animator(unsigned framesX, unsigned framesY, float time, sf::Vector2i frameSize, sf::Sprite& sprite, unsigned YStart) {
	unsigned frameCount = framesX * (framesY > 0 ? framesY : 1);

	m_updateTime = time;

	m_sprite = &sprite;
	m_frames = new sf::IntRect[frameCount];

	frameCount = frameCount;

	CutFrames(framesX, framesY, YStart, frameSize);
}

void Animator::Create(unsigned framesX, unsigned framesY, float time, sf::Vector2i frameSize, sf::Sprite& sprite, unsigned YStart) {
	if (m_frameCount > 0)
		return;

	unsigned frameCount = framesX * (framesY > 0 ? framesY : 1);

	m_updateTime = time;

	m_sprite = &sprite;
	m_frames = new sf::IntRect[frameCount];

	m_frameCount = frameCount;

	CutFrames(framesX, framesY, YStart, frameSize);
}

void Animator::Update(const float deltaTime) {
	m_elapsedTime += deltaTime;
	while (m_elapsedTime >= m_updateTime) {
		m_elapsedTime -= m_updateTime;
		UpdateAnimation();
	}
}

unsigned Animator::GetCurrentFrame() const {
	return m_currentFrame;
}

unsigned Animator::GetFrameCount() const {
	return m_frameCount;
}

void Animator::SetFrame(const unsigned& frame) {
	m_currentFrame = frame;
	m_sprite->setTextureRect(m_frames[m_currentFrame]);
}

void Animator::SetUpdateTime(const float& time) {
	m_updateTime = time;
}

Animator::~Animator() {
	delete[] m_frames;
}

void Animator::UpdateAnimation() {
	if (++m_currentFrame > m_frameCount - 1)
		m_currentFrame = 0;
	m_sprite->setTextureRect(m_frames[m_currentFrame]);
}

void Animator::CutFrames(const unsigned& framesX, const unsigned& framesY, const unsigned& YStart, const sf::Vector2i& frameSize) {
	if (framesY > 0) {
		unsigned iterator = 0;
		for (unsigned y = YStart; y < YStart + framesY; y++) {
			for (unsigned x = 0; x < framesX; x++) {
				m_frames[iterator] = sf::IntRect(x * frameSize.x, y * frameSize.y, frameSize.x, frameSize.y);
				iterator++;
			}
		}
	}
	else {
		for (unsigned i = 0; i < framesX; i++) 
			m_frames[i] = sf::IntRect(i * frameSize.x, YStart * frameSize.y, frameSize.x, frameSize.y);
	}
}