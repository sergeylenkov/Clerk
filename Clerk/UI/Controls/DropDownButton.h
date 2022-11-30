#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/renderer.h>

class DropDownButton : public wxPanel
{
public:
	DropDownButton(wxWindow *parent, wxWindowID id, const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~DropDownButton();

	wxMenu *GetMenu();
	void ClearMenu();

protected:
	void OnKillFocus(wxFocusEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnMenuClose(wxMenuEvent& event);
	void OnPaint(wxPaintEvent &event);
	bool Enable(bool enable = true) override;

private:
	wxBitmap image;
	wxString label;
	wxMenu *menu = nullptr;

	int state = 0;	
	bool IsEnabled = true;
	bool IsMenuVisible = false;	
	const int arrowButtonWidth = 16;
};