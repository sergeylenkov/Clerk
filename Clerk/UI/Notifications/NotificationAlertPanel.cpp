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

	Bind(wxEVT_PAINT, &NotificationAlertPanel::OnPaint, this);
}

void NotificationAlertPanel::SetAlert(std::shared_ptr<AlertPresentationModel> alert) {
	_alert = alert;

	Update();
}

void NotificationAlertPanel::Update() {
	wxString label = _alert->message;

	if (_alert->message.Length() == 0) {
		label = wxString::Format("%s '%s' %s %s", _alert->typeName, _alert->name, _alert->conditionName.Lower(), Format::Amount(_alert->amount));
	}

	_label->SetLabel(label);
	_label->Wrap(GetSize().GetWidth());
}

void NotificationAlertPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxSize size = GetSize();

	wxColor color = Colors::ColorForImportance(_alert->importance);

	dc.SetPen(wxPen(color, 1));
	dc.SetBrush(wxBrush(color));

	dc.DrawRectangle(0, 0, 3, size.GetHeight());
}

void NotificationAlertPanel::OnDismissClick(wxCommandEvent& event) {
	if (OnDismiss) {
		OnDismiss(_alert);
	}
}