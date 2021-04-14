#include "pch.h"
#include "Spectrum.h"

#include <gl/gl.h>
#include <gl/glu.h>

Spectrum::Spectrum(winampVisModule* mod, RECT dimensions, int bars)
{
	m_mod = mod;

	m_dimensions = dimensions;
	m_bars = bars;

	m_spectrumBars = new float[m_bars];

	// Proportion of peak data and spectrum dimensions
	int height = m_dimensions.bottom - m_dimensions.top;
	m_barScale = height / (288.0f / (m_bars / 2.0f) * 255);
}

Spectrum::~Spectrum()
{
	delete m_spectrumBars;
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

	// Left (reverse fill for mirror effect)
	int div = 288 / (m_bars / 2);
	for (int i = 0; i < 288; i++) {
		m_spectrumBars[(288 - i) / div] += m_mod->spectrumData[0][i];
#ifdef DEBUG
		//m_log << "col " << i << ": " << (int)m_mod->spectrumData[0][i] << std::endl;
#endif
	}

	// Right
	for (int i = 0; i < 288; i++) {
		m_spectrumBars[18 + i / div] += m_mod->spectrumData[1][i];
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
		x = 40 + barWidth * i;
		y = 25 + m_spectrumBars[i] * m_barScale;
		
#ifdef DEBUG
		//m_log << "col " << i << ": " << m_spectrumBars[i] << std::endl;
		m_log << "col " << i << " x: " << x << " y: " << y << std::endl;
#endif

		glRectf(
			x,
			25,
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