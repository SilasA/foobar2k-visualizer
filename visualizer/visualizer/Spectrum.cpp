#include "pch.h"
#include "Spectrum.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include <algorithm>
#include <cmath>

#define DATA_SIZE 576

#define SPECTRUM_BEGIN 5
#define SPECTRUM_END 517
#define SPECTRUM_RANGE (SPECTRUM_END - SPECTRUM_BEGIN)

float Spectrum::UpdateCurrentAverage() 
{
	// Convert traditional arrays and add new data
	std::array<float, DATA_SIZE> temp[2];

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

	// Average history data
	for (spec_iter it0 = m_spectrumRolling[0].begin(), it1 = m_spectrumRolling[1].begin();
		it0 != m_spectrumRolling[0].end() && it1 != m_spectrumRolling[1].end();
		it0++, it1++)
	{
		for (int i = 0; i < DATA_SIZE; i++) {
			m_spectrumRollingAvg[0][i] += (*it0)[i] / (float)m_spectrumRolling[0].size();
			m_spectrumRollingAvg[1][i] += (*it1)[i] / (float)m_spectrumRolling[1].size();
		}
	}
}

Spectrum::Spectrum(winampVisModule* mod, RECT dimensions, int bars, int rollingAvgSize)
{
	m_mod = mod;

	m_dimensions = dimensions;
	m_bars = bars;

	m_rollingMaxSize = rollingAvgSize;
	
	// Visual and compiled bar data
	m_spectrumBars = new float[m_bars];

	int width = (m_dimensions.right - m_dimensions.left) / m_bars;
	for (int i = 0; i < m_bars; i++) {
		m_peaks.push_back(Peak({
			m_dimensions.left + i * width, 1,
			m_dimensions.left + (i + 1) * width, 0
			}, GRAVITY));
	}

	// Proportion of peak data and spectrum dimensions
	int height = m_dimensions.top - m_dimensions.bottom;
	m_barScale = height / (std::log10((float)SPECTRUM_RANGE / (m_bars / 2.0f) * 255 + 100) - std::log10(100.0f));
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

	m_last = TIME_CAST(std::chrono::system_clock::now());
#endif

	return SUCCESS;
}

int Spectrum::Render()
{
#ifdef DEBUG
	auto interval = TIME_CAST(std::chrono::system_clock::now()) - m_last;
	m_log << "Frame time: " << DURATION_CAST(interval).count() << " FPS: " << 1000.0 / (DURATION_CAST(interval).count()) << std::endl;
	m_last = TIME_CAST(std::chrono::system_clock::now());
#endif

	// Update current averages
	UpdateCurrentAverage();

	// Clear bar data
	for (int i = 0; i < m_bars; i++) {
		m_spectrumBars[i] = 0;
	}

	// Left (reverse fill for mirror effect)
	int div = SPECTRUM_RANGE / (m_bars / 2);
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(SPECTRUM_RANGE - i - 1) / div] += m_spectrumRollingAvg[0][i + SPECTRUM_BEGIN];
	}

	// Right
	for (int i = 0; i < SPECTRUM_RANGE; i++) {
		m_spectrumBars[(m_bars + i) / div] += m_spectrumRollingAvg[1][i + SPECTRUM_BEGIN];
	}

	int barWidth = (m_dimensions.right - m_dimensions.left) / m_bars;

	// Draw
	for (int i = 0; i < m_bars; i++) {
		float x, y, z = 0;

		x = m_dimensions.left + barWidth * i;
		float logLevel = m_spectrumBars[i] > 0 ? std::log10(m_spectrumBars[i] + 100) : 0;
		y = m_dimensions.bottom + (m_spectrumBars[i] < 1 ? 0 : logLevel - log10(100.f)) * m_barScale;

		// Render peak
		m_peaks[i].Update(y);
		m_peaks[i].Render();

		// Base color
		glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		glVertex3f(x, m_dimensions.bottom, 0);
		glVertex3f(x + barWidth, m_dimensions.bottom, 0);

		// Peak Color
		float max_height = m_dimensions.top - m_dimensions.bottom;
		if (y > m_dimensions.bottom + max_height * .9) {
			float extY = m_dimensions.bottom + max_height * .9;
			glVertex3f(x + barWidth, extY, 0);
			glVertex3f(x, extY, 0);
			glEnd();
			glBegin(GL_POLYGON);
			glColor3f(1, 0, 0);
			glVertex3f(x, extY, 0);
			glVertex3f(x + barWidth, extY, 0);

			glColor3f(1, 0, 0);
			glVertex3f(x + barWidth, y, 0);
			glVertex3f(x, y, 0);
		}
		else {
			glVertex3f(x + barWidth, y, 0);
			glVertex3f(x, y, 0);
		}

		/*glRectf(
			x,
			m_dimensions.bottom,
			x + barWidth,
			y
		);*/
		glEnd();
	}

	return SUCCESS;
}

void Spectrum::Quit() 
{
#ifdef DEBUG
	m_log.close();
#endif
}