#pragma once

#include <wx/wx.h>
#include "TreeMenu.h"

class DataPanel : public wxPanel
{
public:
	DataPanel(wxWindow *parent, wxWindowID id);
	void Update();

	TreeMenuItemTypes type;
	int id;
};