#include "NotificationAlertPanel.h"

NotificationAlertPanel::NotificationAlertPanel(wxWindow* parent, const wxPoint& position, const wxSize& size) : wxPanel(parent, wxID_ANY, position, size)
{
	SetBackgroundColour(wxColor(255, 255, 255));

	SetSize(size);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_label = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, size);

	mainSizer->Add(_label, 0, wxALL, FromDIP(10));

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* button = new wxButton(this, wxID_ANY, _("Dismiss"));
	horizontalSizer->Add(button, 0, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(10));

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT);

	SetSizer(mainSizer);
	Layout();

	button->Bind(wxEVT_BUTTON, &NotificationAlertPanel::OnDismissClick, this);
}

void NotificationAlertPanel::SetAlert(std::shared_ptr<AlertPresentationModel> alert) {
	_alert = alert;

	Update();
}

void NotificationAlertPanel::Update() {
	wxString label = wxString::Format("%s '%s' %s %s", _alert->typeName, _alert->name, _alert->conditionName.Lower(), Format::Amount(_alert->amount));
	_label->SetLabel(label);
	_label->Wrap(GetSize().GetWidth());
}

void NotificationAlertPanel::OnDismissClick(wxCommandEvent& event) {
	if (OnDismiss) {
		OnDismiss(_alert);
	}
}