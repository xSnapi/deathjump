#pragma once
#include "stdafx.h"

class HealthBar : public sf::Drawable {
public:
	HealthBar();
	~HealthBar();

	void Change(bool x); // god please ; - ; 

	int GetHealthState() const;
private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	unsigned m_currentCoords = 0;

	unsigned m_coords[3] =
	{
		0,
		32,
		64
	};

	void InitBody();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};