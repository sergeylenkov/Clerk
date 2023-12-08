#include "NotificationsPopup.h"

NotificationsPopup::NotificationsPopup(wxWindow* parent) : wxPopupTransientWindow(parent, wxBORDER_SIMPLE) {
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_panel->SetBackgroundColour(*wxLIGHT_GREY);

	mainSizer->Add(_panel, 0, wxEXPAND, 0);

	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* text = new wxStaticText(_panel, wxID_ANY, "Test");

	verticalSizer->Add(text);

	_panel->SetSizer(verticalSizer);
	_panel->Layout();

	SetSizer(mainSizer);
	Layout();
}

void NotificationsPopup::Position(wxPoint position, wxSize size) {
	wxPopupWindow::Position(position, size);

	SetSize(size);
	Layout();
}

void NotificationsPopup::Update() {

}