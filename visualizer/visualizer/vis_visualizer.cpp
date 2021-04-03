// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include "vis_visualizer.h"

char description[] = "Theatrical Visualizer";

void config(struct winampVisModule* this_mod);
int init(struct winampVisModule* this_mod);
int render(struct winampVisModule* this_mod);
void quit(struct winampVisModule* this_mod);

winampVisModule mod = {
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
};

winampVisModule* getModule(int which) {
	switch (which) {
	case 0:
		return &mod;
	default:
		return NULL;
	}
}

winampVisHeader header = { VERSION, description, getModule };

static HINSTANCE GetMyInstance() {
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (VirtualQuery(GetMyInstance, &mbi, sizeof(mbi)))
		return (HINSTANCE)mbi.AllocationBase;
	return NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void config(struct winampVisModule* this_mod) {
	MessageBox(getModule(0)->hwndParent, L"Configuration...", L"", MB_OK);
}

int init(struct winampVisModule* this_mod) {
	MessageBox(getModule(0)->hwndParent, L"Init... Successfully installed", L"", MB_OK);
	return 0;
}

int render(struct winampVisModule* this_mod) {
	//MessageBox(getModule(0)->hwndParent, L"Rendering...", L"", MB_OK);
	return 0;
}

void quit(struct winampVisModule* this_mod) {
	MessageBox(getModule(0)->hwndParent, L"Quitting...", L"", MB_OK);
}

extern "C" __declspec(dllexport) winampVisHeader* winampVisGetHeader(HWND hwndParent) {
	return &header;
}
