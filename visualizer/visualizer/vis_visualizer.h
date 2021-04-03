#ifndef VIS_VISUALIZER_H
#define VIS_VISUALIZER_H

#include <windows.h>

#define VERSION 0x10

#define PLUGIN_NAME "Winamp Theatrical Visualizer"

typedef struct winampVisModule {
	char* description;		// Description of module
	HWND hwndParent;		// Parent window (filled by app)
	HINSTANCE hDllInstance; // Instance handle (filled by app)
	int sRate;				// Sample rate (filled by app)
	int nCh;				// Number of Channels
	int latencyMs;			// Latency from call of RenderFrame to actual drawing
	int delayMs;			// Delay between calls in ms

	int spectrumNch;
	int waveformNch;
	unsigned char spectrumData[2][576];
	unsigned char waveformData[2][576];

	void(*Config)(struct winampVisModule* this_mod);	// Configuration dialog
	int(*Init)(struct winampVisModule* this_mod);		// 0 on success, creates window, etc...
	int(*Render)(struct winampVisModule* this_mod);		// Returns 0 if successful, 1 if vis should end
	void(*Quit)(struct winampVisModule* this_mod);		// Call when done
} winampVisModule;

typedef struct {
	int version;
	char* description;
	winampVisModule* (*getModule)(int);
} winampVisHeader;

#endif // VIS_VISUALIZER_H