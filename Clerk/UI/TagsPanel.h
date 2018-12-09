#include <wx/wx.h>
#include "DataPanel.h"

#pragma once

class TagsPanel : public DataPanel
{
public:
	TagsPanel(wxWindow *parent, wxWindowID id);
	~TagsPanel();

	void Update();

private:
	wxTextCtrl *searchField;
	wxScrolledWindow *scrolledWindow;
};

