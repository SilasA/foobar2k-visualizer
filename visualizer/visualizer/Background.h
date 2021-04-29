#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "vis_visualizer.h"
#include "pch.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <deque>

#define LOW 20
#define HIGH 400

#define THRESHOLD 20

class Background
{
private:
	RECT m_dimensions;
	winampVisModule* m_mod;

	std::deque<int> m_levelAvg;

public:
	Background(winampVisModule* mod, RECT dimensions);

	int Init();
	int Render();
	void Quit();
};

#endif // BACKGROUND_H