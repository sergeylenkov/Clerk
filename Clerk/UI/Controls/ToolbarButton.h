#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/renderer.h>

class ToolbarButton : public wxPanel
{
public:
	ToolbarButton(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);	

protected:
	void OnKillFocus(wxFocusEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	bool Enable(bool enable = true) override;
	void ProcessClick();

	int _state = 0;
	bool _isEnabled = true;
};