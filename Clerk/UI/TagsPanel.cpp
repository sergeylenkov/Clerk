#include "TagsPanel.h"

TagsPanel::TagsPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *topPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(topPanel, wxID_ANY, wxT("Search:"), wxDefaultPosition, wxDefaultSize, 0);	
	boxSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	searchField = new wxTextCtrl(topPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0);
	boxSizer->Add(searchField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	topPanel->SetSizer(boxSizer);
	topPanel->Layout();

	boxSizer->Fit(topPanel);
	mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);

	wxPanel *scrollPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	boxSizer = new wxBoxSizer(wxVERTICAL);

	scrolledWindow = new wxScrolledWindow(scrollPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	scrolledWindow->SetScrollRate(5, 5);

	boxSizer->Add(scrolledWindow, 1, wxEXPAND | wxALL, 0);

	scrollPanel->SetSizer(boxSizer);
	scrollPanel->Layout();

	boxSizer->Fit(scrollPanel);
	mainSizer->Add(scrollPanel, 1, wxEXPAND | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();
}

TagsPanel::~TagsPanel() {
	
}

void TagsPanel::Update() {
	int y = 0;
	
	for each (auto tag in DataHelper::GetInstance().GetTags())
	{
		wxStaticText *tagLabel = new wxStaticText(scrolledWindow, wxID_ANY, wxT(""), wxPoint(10, y), wxSize(100, 20), 0);
		tagLabel->SetLabelText(wxString::Format("%s (%d)", *tag->name, tag->count));

		y = y + 30;
	}

	scrolledWindow->FitInside();
	//scrolledWindow->SetVirtualSize(wxSize(100, y));
}