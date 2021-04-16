#include "pch.h"
#include "Spectrum.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <cmath>
#include <algorithm>

#define DATA_SIZE 576

#define SPECTRUM_BEGIN 3
#define SPECTRUM_END 515
#define SPECTRUM_RANGE (SPECTRUM_END - SPECTRUM_BEGIN)

float Spectrum::UpdateCurrentAverage() 
{
	// Convert traditional arrays and add new data
	std::array<float, DATA_SIZE> temp[2];

	// Left
	for (int i = 0; i < DATA_SIZE; i++) {
		temp[0][i] = (float)m_mod->spectrumData[0][i];
		temp[1][i] = (float)m_mod->spectrumData[1][i];
	}
	m_spectrumRolling[0].push_back(temp[0]);
	m_spectrumRolling[1].push_back(temp[1]);

	// Pop oldest (once full)
	// both maintain the same size
	if (m_spectrumRolling[0].size() > m_rollingMaxSize) {
		m_spectrumRolling[0].pop_front();
		m_spectrumRolling[1].pop_front();
	}

	// Clear averaged data
	for (int i = 0; i < DATA_SIZE; i++) {
		m_spectrumRollingAvg[0][i] = 0;
		m_spectrumRollingAvg[1][i] = 0;
	}

	// Sum history data
	for (spec_iter it0 = m_spectrumRolling[0].begin(), it1 = m_spectrumRolling[1].begin();
		it0 != m_spectrumRolling[0].end() && it1 != m_spectrumRolling[1].end();
		it0++, it1++)
	{
		for (int i = 0; i < DATA_SIZE; i++) {
			m_spectrumRollingAvg[0][i] += (*it0)[i];
			m_spectrumRollingAvg[1][i] += (*it1)[i];
		}
	}

	// Make average
	for (int i = 0; i < DATA_SIZE; i++) {
		m_spectrumRollingAvg[0][i] /= (float)m_spectrumRolling[0].size();
		m_spectrumRollingAvg[1][i] /= (float)m_spectrumRolling[1].size();
	}
}

Spectrum::Spectrum(winampVisModule* mod, RECT dimensions, int bars, int rollingAvgSize)
{
	m_mod = mod;

	m_dimensions = dimensions;
	m_bars = bars;

	m_rollingMaxSize = rollingAvgSize;
	
	// Visual and compiled bar data
	m_spectrumBars = new float[bars];

	// Proportion of peak data and spectrum dimensions
	int height = m_dimensions.top - m_dimensions.bottom;
	m_barScale = height / std::log10((float)SPECTRUM_RANGE / (m_bars / 2.0f) * 255);
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
	// Update current averages
	UpdateCurrentAverage();

	// Clear bar data
	for (int i = 0; i < m_bars; i++) {
		m_spectrumBars[i] = 0;
	}

	// Goal:
	// Map 0->576 x2 => 3->515 x2

	// Left (reverse fill for mirror effect)
	int div = SPECTRUM_RANGE / (m_bars / 2);
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(SPECTRUM_RANGE - i - 1) / div] += m_spectrumRollingAvg[0][i + SPECTRUM_BEGIN];
#ifdef DEBUG
		//m_log << "col " << i << ": " << (int)m_mod->spectrumData[0][i] << std::endl;
#endif
	}

	// Right
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(m_bars + i) / div] += m_spectrumRollingAvg[1][i + SPECTRUM_BEGIN];
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