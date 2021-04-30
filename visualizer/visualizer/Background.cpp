#include "Background.h"

#include <algorithm>

Background::Background(winampVisModule* mod, RECT dimensions)
{
	m_mod = mod;
	m_dimensions = dimensions;
}

int Background::Init() 
{
	m_levelAvg.push_back(0);
	return SUCCESS;
}

int Background::Render()
{
	// Rudementary beat detection
	int cur = 0;
	int average = 0;

	// Average specific range of spectrums
	for (int i = LOW; i < HIGH; i++) {
		cur += ((int)m_mod->spectrumData[0][i] + (int)m_mod->spectrumData[1][i]) / 2;
	}
	cur /= HIGH - LOW;

	// High-pass filter
	cur = cur > THRESHOLD ? cur : 0;

	// Rolling average
	m_levelAvg.push_back(cur);
	if (m_levelAvg.size() > 5)
		m_levelAvg.pop_front();
	
	for (std::deque<int>::iterator it = m_levelAvg.begin();
		it != m_levelAvg.end();
		it++) {
		average += *it;
	}
	average /= m_levelAvg.size();

	// Flash threshold
	int output = std::log10(cur) > std::log10(average) * 1.05 ? .8 * cur : 0;

	// Draw background
	glBegin(GL_QUADS);
	glColor4ub(output, output, output, 100);
	glVertex3f(0, 0, 0);
	glVertex3f(0, DEFAULT_WINDOW_HEIGHT, 0);
	glVertex3f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0);
	glVertex3f(DEFAULT_WINDOW_WIDTH, 0, 0);
	glEnd();

	return SUCCESS;
}

void Background::Quit()
{
}