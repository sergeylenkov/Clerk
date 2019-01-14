#pragma once

#include <wx/wx.h>
#include "../Defines.h"

class AboutFrame : public wxFrame
{
public:
	AboutFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AboutFrame();
};

