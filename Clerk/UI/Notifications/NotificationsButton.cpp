#include "NotificationsButton.h"

using namespace Clerk::UI;

NotificationsButton::NotificationsButton(wxWindow* parent, const wxPoint& pos, const wxSize& size) : 
	ToolbarButton(parent, wxID_ANY, pos, size) {
	_image = wxBitmap(wxT("ICON_BELL"), wxBITMAP_TYPE_PNG_RESOURCE);
	_imageActive = wxBitmap(wxT("ICON_BELL_ACTIVE"), wxBITMAP_TYPE_PNG_RESOURCE);

	_popup = new NotificationsPopup(this);

	Bind(wxEVT_PAINT, &NotificationsButton::OnPaint, this);
	Bind(wxEVT_BUTTON, &NotificationsButton::OnClick, this);
}

void NotificationsButton::OnPaint(wxPaintEvent& event)
{
	ToolbarButton::OnPaint(event);

	wxPaintDC dc(this);
	wxSize size = GetSize();
	wxSize imageSize = _image.GetSize();

	int x = (size.GetWidth() - imageSize.GetWidth()) / 2;
	int y = (size.GetHeight() - imageSize.GetHeight()) / 2;

	dc.DrawBitmap(_image, x, y, true);
}

void NotificationsButton::OnClick(wxCommandEvent& event) {
	if (_popup->IsShown()) {
		_popup->Hide();
		return;
	}

	wxPoint position = GetScreenPosition();
	wxSize size = GetSize();
	wxSize panelSize = wxSize(FromDIP(300), FromDIP(300));

	_popup->Position(wxPoint(position.x + size.GetWidth(), (position.y - panelSize.GetHeight()) + size.GetHeight()), panelSize);
	_popup->Show();
}