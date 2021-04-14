#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "vis_visualizer.h"

#ifdef DEBUG
#include <fstream>
#endif

class Spectrum
{
private:
	float*	m_spectrumBars;
	int		m_bars;
	float	m_barScale;

	RECT m_dimensions;

	winampVisModule* m_mod;

#ifdef DEBUG
	std::ofstream m_log;
#endif

public:
	Spectrum(winampVisModule* mod, RECT dimensions, int bars);
	~Spectrum();

	int Init();
	int Render();
	void Quit();
};

#endif // SPECTRUM_H
