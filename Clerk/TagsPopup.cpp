#include "TagsPopup.h"

TagsPopup::TagsPopup(wxWindow *parent) :wxPopupWindow(parent) {
	panel = new wxScrolledWindow(this, wxID_ANY);
	panel->SetBackgroundColour(*wxLIGHT_GREY);
	
	list = new wxListBox(panel, wxID_ANY, wxPoint(0, 0), wxSize(200, 100));
	list->AppendString(wxString("Test 1"));
	list->AppendString(wxString("Test 2"));
	list->AppendString(wxString("Test 3"));

	/*wxStaticText *text = new wxStaticText(m_panel, wxID_ANY,
		wxT("wxPopupTransientWindow is a\n")
		wxT("wxPopupWindow which disappears\n")
		wxT("automatically when the user\n")
		wxT("clicks the mouse outside it or if it\n")
		wxT("any other way."));*/

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(list, 0, wxALL, 5);

	panel->SetAutoLayout(true);
	panel->SetSizer(topSizer);

	topSizer->Fit(panel);

	SetClientSize(panel->GetSize());
}

TagsPopup::~TagsPopup() {
}

void TagsPopup::Update(vector<shared_ptr<wxString>> tags) {
	list->Clear();

	for each (auto tag in tags) {
		list->Append(*tag.get());
	}

	if (list->GetCount() > 0) {
		list->Select(0);
	}
}

void TagsPopup::SelectNext() {
	if (list->GetCount() < 2) {
		return;
	}

	int index = list->GetSelection();

	index++;

	if (index >= list->GetCount()) {
		index = list->GetCount() - 1;
	}

	list->Select(index);
}

void TagsPopup::SelectPrev() {
	if (list->GetCount() < 2) {
		return;
	}

	int index = list->GetSelection();

	index--;

	if (index < 0) {
		index = 0;
	}

	list->Select(index);
}

wxString TagsPopup::GetSelectedTag() {
	if (!list->IsEmpty()) {
		return list->GetStringSelection();
	}

	return wxString("");
}