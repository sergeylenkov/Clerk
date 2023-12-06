#include "AboutDialog.h"

AboutDialog::AboutDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxString version = wxString::Format(_("Version %s"), APP_VERSION);

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBitmap *logoImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap("APP_ICON_BIG", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(-1, -1), 0);
	verticalSizer->Add(logoImage, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, this->FromDIP(10));

	wxStaticText *nameLabel = new wxStaticText(this, wxID_ANY, APP_NAME);
	nameLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	verticalSizer->Add(nameLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, this->FromDIP(5));

	wxStaticText *versionLabel = new wxStaticText(this, wxID_ANY, version);
	verticalSizer->Add(versionLabel, 0, wxALIGN_CENTER_HORIZONTAL);

	mainSizer->Add(verticalSizer, 1, wxEXPAND);

	verticalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *copyrightLabel = new wxStaticText(this, wxID_ANY, _("Copyright © 2024 Sergey Lenkov"));
	verticalSizer->Add(copyrightLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, this->FromDIP(10));

	mainSizer->Add(verticalSizer, 0, wxEXPAND);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
}
