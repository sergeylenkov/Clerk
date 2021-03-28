#include "AlertsConfirmDialog.h"

AlertsConfirmDialog::AlertsConfirmDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *titleLabel = new wxStaticText(this, wxID_ANY, wxT("Alerts:"), wxDefaultPosition, wxDefaultSize, 0);
	mainSizer->Add(titleLabel, 0, wxALL, 10);

	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	mainSizer->Add(list, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	wxPanel *buttonsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(buttonsPanel, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	buttonsPanel->SetSizer(horizontalSizer);
	buttonsPanel->Layout();

	horizontalSizer->Fit(buttonsPanel);

	mainSizer->Add(buttonsPanel, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	okButton->Bind(wxEVT_BUTTON, &AlertsConfirmDialog::OnOk, this);
	Bind(wxEVT_CLOSE_WINDOW, &AlertsConfirmDialog::OnCloseWindow, this);
}

AlertsConfirmDialog::~AlertsConfirmDialog()
{
}

void AlertsConfirmDialog::SetAlerts(std::vector<std::shared_ptr<AlertViewModel>> alerts) {
	this->alerts = alerts;
	UpdateList();
}

void AlertsConfirmDialog::UpdateList() {
	list->ClearAll();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	list->InsertColumn(0, column);

	int i = 0;

	for (auto &alert : alerts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(alert->id);

		list->InsertItem(listItem);

		wxString title = wxString::Format("%s '%s' %s %s", alert->typeName, alert->name, alert->conditionName.Lower(), Format::Amount(alert->amount));

		list->SetItem(i, 0, title);
		i++;
	}
}

void AlertsConfirmDialog::OnOk(wxCommandEvent &event) {
	Close();
}

void AlertsConfirmDialog::OnCloseWindow(wxCloseEvent &event) {
	if (OnClose) {
		OnClose();
	}

	Destroy();
}