#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "vis_visualizer.h"

#include <deque>
#include <array>

#ifdef DEBUG
#include <fstream>
#endif

typedef std::deque<std::array<float, 576>>::iterator spec_iter;

class Spectrum
{
private:
	std::deque<std::array<float, 576>> m_spectrumRolling[2];

	float	m_spectrumRollingAvg[2][576];
	float*	m_spectrumBars;
	int		m_bars;
	int		m_rollingMaxSize;
	float	m_barScale;

	RECT m_dimensions;

	winampVisModule* m_mod;

	// Rolling average functions
	float UpdateCurrentAverage();

#ifdef DEBUG
	std::ofstream m_log;
#endif

public:
	Spectrum(winampVisModule* mod, RECT dimensions, int bars, int rollingAvgSize = 5);
	~Spectrum();

	int Init();
	int Render();
	void Quit();
};

#endif // SPECTRUM_H
