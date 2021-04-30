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

// Manager for an N-band spectrum visualizer
class Spectrum
{
private:
	// History of spectral data
	std::deque<std::array<float, DATA_SIZE>> m_spectrumRolling[2];

	// Peak caps
	std::vector<Peak> m_peaks;

	// Current average from spectral history data
	float	m_spectrumRollingAvg[2][DATA_SIZE];
	float*	m_spectrumBars;
	int		m_bars;
	int		m_rollingMaxSize;
	float	m_barScale;

	RECT m_dimensions;

	winampVisModule* m_mod;

	// Overlay data
	char* m_pluginDir;
	GLuint m_overlay;
	float m_scaleTexWidth, m_scaleTexHeight;

#ifdef DEBUG
	TIME_MS m_last;
#endif

	// Computes the current rolling average from spectral history data
	float UpdateCurrentAverage();

#ifdef DEBUG
	std::ofstream m_log;
#endif

public:
	// Creates the visual spectrum
	//  mod - pointer to the module data structure (for audio spectrum data)
	//  dimensions - rectangle where the spectrum will reside on the screen
	//  bars - how many bars the spectrum will have (must be even)
	//  rollingAvgSize - (Optional) size of buffer for rolling average
	Spectrum(winampVisModule* mod, RECT dimensions, int bars, int rollingAvgSize = 5);
	~Spectrum();

	int Init(char* pluginDir);
	int Render();
	void Quit();
};

#endif // SPECTRUM_H
