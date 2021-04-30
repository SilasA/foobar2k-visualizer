#ifndef VIS_VISUALIZER_H
#define VIS_VISUALIZER_H

#include <windows.h>
#include <Winamp/wa_ipc.h>

#define VERSION 0x10

#define PLUGIN_NAME "Winamp Spectrum Visualizer"


// Complete structure of all data Winamp needs to run the plugin
typedef struct winampVisModule {
	char* description;		// Description of module
	HWND hwndParent;		// Parent window (filled by app)
	HINSTANCE hDllInstance; // Instance handle (filled by app)
	int sRate;				// Sample rate (filled by app)
	int nCh;				// Number of Channels
	int latencyMs;			// Latency from call of RenderFrame to actual drawing
	int delayMs;			// Delay between calls in ms

	int spectrumNch;		// Number of channels to get (l, r, stereo) from Winamp
	int waveformNch;		// Number of channels to get (l, r, stereo) from Winamp
	unsigned char spectrumData[2][576];	// Spectral data from 0-20000Hz
	unsigned char waveformData[2][576]; // Waveform sample data

	// Plugin functions defined in vis_visualizer.cpp
	void(*Config)(struct winampVisModule* this_mod);	// Configuration dialog
	int(*Init)(struct winampVisModule* this_mod);		// 0 on success, creates window, etc...
	int(*Render)(struct winampVisModule* this_mod);		// Returns 0 if successful, 1 if vis should end
	void(*Quit)(struct winampVisModule* this_mod);		// Call when done
} winampVisModule;

// Header passed to Winamp containing a function to get the module structure
typedef struct {
	int version;
	char* description;
	winampVisModule* (*getModule)(int);
} winampVisHeader;

// Structure for all plugin data and window handle data
typedef struct {
	winampVisHeader hdr;	// Plugin header
	winampVisModule mod;	// Plugin module

	HDC hDC;				// canvas
	HGLRC hRC;				// 3d canvas
	HWND hWnd;				// window handle

	embedWindowState windowState;
} winampVisualizer;

// Prototypes
winampVisualizer* getVisInstance();
winampVisModule* getModule(int which);

// Called when configure is pressed in winamp/foobar
void config(struct winampVisModule* this_mod);

// Called when the plugin is started
int init(struct winampVisModule* this_mod);

// Called periodically based on delayMs
int render(struct winampVisModule* this_mod);

// Called when plugin is stopped
void quit(struct winampVisModule* this_mod);

void resizeWindow(int width, int height);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // VIS_VISUALIZER_H