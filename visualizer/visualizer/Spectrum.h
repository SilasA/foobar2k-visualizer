#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "vis_visualizer.h"
#include "Peak.h"
#include "libpng/lodepng.h"

#include <array>
#include <chrono>
#include <deque>
#include <vector>

#include <gl/gl.h>
#include <gl/glu.h>

#ifdef DEBUG
#include <fstream>
#endif

#define DATA_SIZE 576

typedef std::deque<std::array<float, DATA_SIZE>>::iterator spec_iter;

class Spectrum
{
private:
	std::deque<std::array<float, DATA_SIZE>> m_spectrumRolling[2];
	std::vector<Peak> m_peaks;

	float	m_spectrumRollingAvg[2][DATA_SIZE];
	float*	m_spectrumBars;
	int		m_bars;
	int		m_rollingMaxSize;
	float	m_barScale;

	RECT m_dimensions;

	winampVisModule* m_mod;

	char* m_pluginDir;
	GLuint m_overlay;
	float m_scaleTexWidth, m_scaleTexHeight;

#ifdef DEBUG
	TIME_MS m_last;
#endif

	// Rolling average functions
	float UpdateCurrentAverage();

#ifdef DEBUG
	std::ofstream m_log;
#endif

public:
	Spectrum(winampVisModule* mod, RECT dimensions, int bars, int rollingAvgSize = 5);
	~Spectrum();

	int Init(char* pluginDir);
	int Render();
	void Quit();
};

#endif // SPECTRUM_H
