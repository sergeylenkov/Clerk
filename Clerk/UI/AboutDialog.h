#pragma once

#include <wx/wx.h>
#include "../Defines.h"

class AboutDialog : public wxFrame
{
public:
	AboutDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
};

