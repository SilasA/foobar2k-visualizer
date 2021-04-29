#include "Background.h"

#include <algorithm>

Background::Background(winampVisModule* mod, RECT dimensions)
{
	m_mod = mod;
	m_dimensions = dimensions;
}

int Background::Init() 
{
	return SUCCESS;
}

int Background::Render()
{
	// Rudementary beat detection

	int cur = 0;
	int level = 0;
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
		level += *it;
	}
	level /= m_levelAvg.size();

	// Cube output for more volatile upper end
	level *= level * level;

	// Draw background
	glBegin(GL_QUADS);
	glColor4ub(level, level, level, 100);
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