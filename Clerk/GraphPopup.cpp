#include "GraphPopup.h"

#include "TagsPopup.h"

GraphPopup::GraphPopup(wxWindow *parent) :wxPopupWindow(parent) {
	panel = new wxScrolledWindow(this, wxID_ANY);
	panel->SetBackgroundColour(*wxLIGHT_GREY);

	list = new wxListBox(panel, wxID_ANY, wxPoint(0, 0), wxSize(200, 100));

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(list, 0, wxALL, 5);

	panel->SetAutoLayout(true);
	panel->SetSizer(topSizer);

	topSizer->Fit(panel);

	SetClientSize(panel->GetSize());
}

GraphPopup::~GraphPopup() {
}

void GraphPopup::Update() {
	list->Clear();

	
}
