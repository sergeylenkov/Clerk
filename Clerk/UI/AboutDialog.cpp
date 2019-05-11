#include "AboutDialog.h"

AboutDialog::AboutDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxString version = wxString::Format("Version %s", APP_VERSION);

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBitmap *logoImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("APP_ICON_BIG"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(-1, -1), 0);
	mainSizer->Add(logoImage, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	wxStaticText *nameLabel = new wxStaticText(this, wxID_ANY, wxT("Clerk"), wxDefaultPosition, wxDefaultSize, 0);
	nameLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	mainSizer->Add(nameLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	wxStaticText *versionLabel = new wxStaticText(this, wxID_ANY, version, wxDefaultPosition, wxDefaultSize, 0);
	mainSizer->Add(versionLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	mainSizer->Add(0, 0, 1, wxEXPAND, 5);

	wxStaticText *copyrightLabel = new wxStaticText(this, wxID_ANY, wxT("Copyright © 2019 Sergey Lenkov"), wxDefaultPosition, wxDefaultSize, 0);
	mainSizer->Add(copyrightLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
}
