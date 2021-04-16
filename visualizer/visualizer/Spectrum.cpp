#include "pch.h"
#include "Spectrum.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <cmath>
#include <algorithm>

#define SPECTRUM_BEGIN 3
#define SPECTRUM_END 515
#define SPECTRUM_RANGE (SPECTRUM_END - SPECTRUM_BEGIN)

Spectrum::Spectrum(winampVisModule* mod, RECT dimensions, int bars)
{
	m_mod = mod;

	m_dimensions = dimensions;
	m_bars = bars;

	m_spectrumBars = new float[m_bars];

	// Proportion of peak data and spectrum dimensions
	int height = m_dimensions.top - m_dimensions.bottom;
	m_barScale = height / std::log2((float)SPECTRUM_RANGE / (m_bars / 2.0f) * 255);
}

Spectrum::~Spectrum()
{
	delete[] m_spectrumBars;
}

int Spectrum::Init() 
{
#ifdef DEBUG
	m_log.open("E:\\Projects\\foobar2k-visualizer\\logs\\log.txt");
	if (!m_log.is_open())
		return FAILURE;
#endif

	return SUCCESS;
}

int Spectrum::Render()
{
	// Clear Spectrum
	for (int i = 0; i < m_bars; i++) {
		m_spectrumBars[i] = 0;
	}

	// Goal:
	// Map 0->576 x2 => 3->515 x2


	// Left (reverse fill for mirror effect)
	int div = SPECTRUM_RANGE / (m_bars / 2);
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(SPECTRUM_RANGE - i - 1) / div] += m_mod->spectrumData[0][i + SPECTRUM_BEGIN];
#ifdef DEBUG
		//m_log << "col " << i << ": " << (int)m_mod->spectrumData[0][i] << std::endl;
#endif
	}

	// Right
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(m_bars + i) / div] += m_mod->spectrumData[1][i + SPECTRUM_BEGIN];
	}

	// TODO: Trim and scale

	int barWidth = (m_dimensions.right - m_dimensions.left) / m_bars;

#ifdef DEBUG
	m_log << "\nWidth = " << barWidth << std::endl;
#endif

	// Draw
	glColor3f(1, 0, 0);
	
	for (int i = 0; i < m_bars; i++) {
		float x, y, z = 0;

		// TODO: replace DEFAULT_WINDOW_WIDTH with current window width
		x = m_dimensions.left + barWidth * i;
		float logLevel = m_spectrumBars[i] > 0 ? std::log10(m_spectrumBars[i]) : 0;
		y = m_dimensions.bottom + (m_spectrumBars[i] < 1 ? 0 : logLevel) * m_barScale;
		
#ifdef DEBUG
		//m_log << "col " << i << ": " << m_spectrumBars[i] << std::endl;
		m_log << "col " << i << " x: " << x << " d: " << m_spectrumBars[i] << " log: " << logLevel << std::endl;
#endif

		glRectf(
			x,
			m_dimensions.bottom,
			x + barWidth,
			y
		);
	}

	return SUCCESS;
}

void Spectrum::Quit() 
{
#ifdef DEBUG
	m_log.close();
#endif
}