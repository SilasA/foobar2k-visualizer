#include "pch.h"
#include "Spectrum.h"
#include "libpng/lodepng.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include <algorithm>
#include <cmath>
#include <string.h>
#include <direct.h>

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
	memset(m_spectrumRollingAvg[0], 0, sizeof(int) * DATA_SIZE);
	memset(m_spectrumRollingAvg[1], 0, sizeof(int) * DATA_SIZE);

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

int Spectrum::Init(char* pluginDir)
{
	m_pluginDir = pluginDir;
	char path[128];
	strcat_s(path, 128, pluginDir);
	strcat_s(path, 128, SPECTRUM_OVERLAY);

	// Load overlay image
	std::vector<unsigned char> image;
	unsigned w, h;
	unsigned error = lodepng::decode(image, w, h, path);

	if (error != 0) {
		return FAILURE;
	}

	size_t width = 2048;
	size_t height = 1024;

	m_scaleTexWidth = (double)w / width;
	m_scaleTexHeight = (double)h / height;

	std::vector<unsigned char> image2(width * height * 4);
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			for (size_t c = 0; c < 4; c++)
				image2[4 * width * y + 4 * x + c] = image[4 * w * y + 4 * x + c];

	// Convert to texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_overlay);
	glBindTexture(GL_TEXTURE_2D, m_overlay);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

#ifdef DEBUG
	m_log.open("E:\\Projects\\foobar2k-visualizer\\logs\\log.txt");
	if (!m_log.is_open())
		return FAILURE;

	m_log << path << std::endl;

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
	memset(m_spectrumBars, 0, sizeof(int) * DATA_SIZE);

	// Map the raw spectrum data to the spectrum bars (often a reduction)
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

	// Draw the overlay
	glBegin(GL_QUADS);
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, m_scaleTexHeight); glVertex3f(0, 0, 0);
	glTexCoord2d(m_scaleTexWidth, m_scaleTexHeight); glVertex3f(DEFAULT_WINDOW_WIDTH, 0, 0);
	glTexCoord2d(m_scaleTexWidth, 0); glVertex3f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0);
	glTexCoord2d(0, 0); glVertex3f(0, DEFAULT_WINDOW_HEIGHT, 0);
	glDisable(GL_TEXTURE_2D);
	glEnd();

	// Draw spectrum
	for (int i = 0; i < m_bars; i++) {
		float x, y, z = 0;

		// Stupid faux-logarithm scale math
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