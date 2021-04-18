#ifndef PEAK_H
#define PEAK_H

#include "pch.h"
#include <windows.h>

class Peak
{
private:
	float m_gravity;
	float m_velocity;
	RECT m_location;

	float m_thickness;

public:
	Peak(RECT location, float gravity);

	void Update(float level, float dt = RENDER_PERIOD);
	void Render();
};

#endif // PEAK_H
