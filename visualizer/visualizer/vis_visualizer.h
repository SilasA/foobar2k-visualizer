#ifndef VIS_VISUALIZER_H
#define VIS_VISUALIZER_H

#include <windows.h>
#include <Winamp/wa_ipc.h>

#define VERSION 0x10

#define PLUGIN_NAME "Winamp Theatrical Visualizer"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

char description[] = "Theatrical Visualizer";

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

void config(struct winampVisModule* this_mod);
int init(struct winampVisModule* this_mod);
int render(struct winampVisModule* this_mod);
void quit(struct winampVisModule* this_mod);

void resizeWindow(int width, int height);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Visualizer data
static winampVisualizer _vis = {
	{
		VERSION,
		description,
		getModule
	},
	{
		description,
		NULL,
		NULL,
		0,
		0,
		0,
		10,
		0,
		2,
		{ 0, },
		{ 0, },
		config,
		init,
		render,
		quit
	},

	NULL,
	NULL,
	NULL
};


winampVisualizer* getVisInstance() {
	return &_vis;
}

winampVisModule* getModule(int which) {
	switch (which) {
	case 0:
		return &_vis.mod;
	default:
		return NULL;
	}
}

#endif // VIS_VISUALIZER_H