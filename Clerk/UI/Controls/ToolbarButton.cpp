#include "ToolbarButton.h"

ToolbarButton::ToolbarButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxPanel(parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, "ToolbarButton")
{
	wxSize defaultSize = wxButton::GetDefaultSize();
	
	defaultSize.SetWidth(defaultSize.GetWidth());
	defaultSize.SetHeight(defaultSize.GetHeight());

	if (size != wxDefaultSize) {
		if (size.GetWidth() != -1) {
			defaultSize.SetWidth(size.GetWidth());
		}

		if (size.GetHeight() != -1) {
			defaultSize.SetHeight(size.GetHeight());
		}
	}

	SetMinSize(defaultSize);

	Bind(wxEVT_PAINT, &ToolbarButton::OnPaint, this);
	Bind(wxEVT_KILL_FOCUS, &ToolbarButton::OnKillFocus, this);
	Bind(wxEVT_LEAVE_WINDOW, &ToolbarButton::OnMouseLeave, this);
	Bind(wxEVT_ENTER_WINDOW, &ToolbarButton::OnMouseEnter, this);
	Bind(wxEVT_LEFT_UP, &ToolbarButton::OnLeftButtonUp, this);
	Bind(wxEVT_LEFT_DOWN, &ToolbarButton::OnLeftButtonDown, this);
}

void ToolbarButton::OnKillFocus(wxFocusEvent& event)
{
	_state = wxCONTROL_NONE;
	Refresh();
}

void ToolbarButton::OnMouseLeave(wxMouseEvent& event)
{
	_state = wxCONTROL_NONE;
	Refresh();
}

void ToolbarButton::OnMouseEnter(wxMouseEvent& event)
{
	_state = wxCONTROL_CURRENT;
	Refresh();
}

void ToolbarButton::OnLeftButtonUp(wxMouseEvent& event) {
	_state = wxCONTROL_NONE;
	Refresh();

	ProcessClick();
}

void ToolbarButton::OnLeftButtonDown(wxMouseEvent& event) {
	_state = wxCONTROL_PRESSED;
	Refresh();
}

void ToolbarButton::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxSize size = GetSize();

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	if (_state == wxCONTROL_NONE) {
		dc.SetPen(wxPen(wxColor(255, 255, 255), 1));
		dc.SetBrush(wxBrush(wxColor(255, 255, 255), wxBRUSHSTYLE_SOLID));
	}
	else if (_state == wxCONTROL_CURRENT) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(228, 242, 250), wxBRUSHSTYLE_SOLID));
	}
	else if (_state == wxCONTROL_PRESSED) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
	} else {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
	}

	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
}

bool ToolbarButton::Enable(bool enable)
{
	_isEnabled = enable;
	wxPanel::Enable(_isEnabled);

	if (_isEnabled)
	{
		_state = wxCONTROL_NONE;
	}
	else
	{
		_state = wxCONTROL_DISABLED;
	}


	Refresh();

	return enable;
}

void ToolbarButton::ProcessClick() {
	wxEvtHandler* eventHandler = GetEventHandler();

	eventHandler->CallAfter([=]() {
		wxCommandEvent evt(wxEVT_BUTTON, this->GetId());
		evt.SetEventObject(this);
		GetEventHandler()->ProcessEvent(evt);
	});
}