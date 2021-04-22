#include "pch.h"
#include "Peak.h"

#include <gl/gl.h>

Peak::Peak(RECT location, float gravity) 
{
	m_gravity = gravity;
	m_location = location;
	m_velocity = 0.0;
	m_thickness = m_location.top - m_location.bottom;
}

void Peak::Update(float level, float dt) 
{
	// Level Decreasing
	if (level < m_location.bottom)
	{
		m_velocity += m_gravity * dt / 1000;
		m_location.top -= m_velocity;
		m_location.bottom -= m_velocity;
	}

	// Level increasing
	else if (level > m_location.bottom) 
	{
		m_velocity = 0.0;
		m_location.bottom = level;
		m_location.top = level + m_thickness;
	}

	// Lower bound
	if (m_location.bottom < 0)
	{
		m_location.bottom = 0.0;
		m_location.top = m_thickness;
	}
}

void Peak::Render() 
{
	glColor3f(1, 1, 1);
	glRectf(
		m_location.left,
		m_location.top,
		m_location.right,
		m_location.bottom
	);
}