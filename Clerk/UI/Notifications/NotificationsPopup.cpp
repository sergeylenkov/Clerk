#include "NotificationsPopup.h"

NotificationsPopup::NotificationsPopup(wxWindow* parent) : wxPopupTransientWindow(parent, wxBORDER_SIMPLE) {
	_panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_panel->SetBackgroundColour(*wxWHITE);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_panel, 0, wxEXPAND, 0);

	_notificationsSizer = new wxBoxSizer(wxVERTICAL);

	_panel->SetSizer(_notificationsSizer);
	_panel->Layout();

	SetSizer(mainSizer);
	Layout();
}

void NotificationsPopup::Position(wxPoint position, wxSize size) {
	wxPopupWindow::Position(position, size);

	SetSize(size);
	Layout();
}

void NotificationsPopup::Update(shared_vector<AlertPresentationModel> alerts) {
	_notificationsSizer->Clear(true);

	wxSize size = wxSize(GetSize().GetWidth(), -1);

	for (auto& alert : alerts) {
		NotificationAlertPanel* alertPanel = new NotificationAlertPanel(_panel, wxDefaultPosition, size);
		alertPanel->SetAlert(alert);

		alertPanel->OnDismiss = [&](std::shared_ptr<AlertPresentationModel> alert) {
			if (OnDismiss) {
				OnDismiss(alert);
			}
		};

		_notificationsSizer->Add(alertPanel);
	}

	_notificationsSizer->Layout();
	wxSize popupSize = _notificationsSizer->GetSize();

	if (popupSize.GetHeight() > FromDIP(300)) {
		popupSize.SetHeight(FromDIP(300));
	}

	SetSize(popupSize);

	Layout();
}