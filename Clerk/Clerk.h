#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <crtdbg.h>

#include "wx/wx.h" 
#include "MainFrame.h"

class ClerkApp : public wxApp {
	virtual bool OnInit();
};