#ifndef PEAK_H
#define PEAK_H

#include "pch.h"
#include <windows.h>

// Manager for a gravity-affected peak cap
class Peak
{
private:
	float m_gravity;
	float m_velocity;
	RECT m_location;

	float m_thickness;

public:
	// Construct a peak cap
	// location - the location and size of the cap (used to compute thickness)
	// gravity - constant acceleration value applied to cap (px / s)
	Peak(RECT location, float gravity);

	// Update the location of the peak cap
	//  level - level data for the associated bar influencing the cap
	//  dt - frame time. defaults to contant render period supplied to winamp
	void Update(float level, float dt = RENDER_PERIOD);
	void Render();
};

#endif // PEAK_H
