#include "TagPanel.h"

TagPanel::TagPanel(wxWindow* parent, const wxPoint& position, const wxSize& size) : wxPanel(parent, wxID_ANY, position, size)
{
	SetBackgroundColour(wxColor(241, 248, 255));

	_mainSizer = new wxBoxSizer(wxHORIZONTAL);

	_label = new wxStaticText(this, wxID_ANY, "");
	_mainSizer->Add(_label, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, FromDIP(5));

	wxButton* button = new wxButton(this, wxID_ANY, L"🞪", wxDefaultPosition, FromDIP(wxSize(20, 20)), wxBORDER_NONE);
	button->SetBackgroundColour(wxColor(241, 248, 255));

	button->Bind(wxEVT_BUTTON, &TagPanel::OnDeleteClick, this);

	_mainSizer->Add(button, 0, wxALIGN_CENTER_VERTICAL);

	SetSizer(_mainSizer);
	Layout();
}

void TagPanel::SetTag(std::shared_ptr<TagPresentationModel> tag) {
	_tag = tag;

	_label->SetLabel(tag->name);
}

void TagPanel::OnDeleteClick(wxCommandEvent& event) {
	if (OnDelete) {
		OnDelete(_tag);
	}
}