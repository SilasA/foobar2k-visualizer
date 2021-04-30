#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "vis_visualizer.h"
#include "pch.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <deque>

#define LOW 5
#define HIGH 25

#define THRESHOLD 35

// Manager for a beat detection background graphic
class Background
{
private:
	RECT m_dimensions;
	winampVisModule* m_mod;

	// Level rolling average
	std::deque<int> m_levelAvg;

public:
	Background(winampVisModule* mod, RECT dimensions);

	int Init();
	int Render();
	void Quit();
};

#endif // BACKGROUND_H