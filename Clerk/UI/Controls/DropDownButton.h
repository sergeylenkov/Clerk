#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/renderer.h>
#include "ToolbarButton.h"

class DropDownButton : public ToolbarButton
{
public:
	DropDownButton(wxWindow *parent, const wxBitmap& bitmap, const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~DropDownButton();

	wxMenu *GetMenu();
	void ClearMenu();

protected:
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnMenuClose(wxMenuEvent& event);
	void OnPaint(wxPaintEvent &event);

private:
	wxBitmap _image;
	wxString _label;
	wxMenu *_menu = nullptr;
	
	bool _isMenuVisible = false;
	int _arrowButtonWidth = 0;
};