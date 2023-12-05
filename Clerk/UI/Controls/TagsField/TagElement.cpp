#include "TagElement.h"

TagElement::TagElement(wxWindow* parent, const wxPoint& position, const wxSize& size) : wxPanel(parent, wxID_ANY, position, size)
{
	SetBackgroundColour(wxColor(241, 248, 255));

	_mainSizer = new wxBoxSizer(wxHORIZONTAL);

	this->SetSizer(_mainSizer);
	this->Layout();
}

void TagElement::SetTag(std::shared_ptr<TagPresentationModel> tag) {
	_tag = tag;

	_mainSizer->Clear(true);

	wxStaticText* label = new wxStaticText(this, wxID_ANY, tag->name);
	_mainSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, this->FromDIP(5));

	wxButton* button = new wxButton(this, wxID_ANY, L"🞪", wxDefaultPosition, this->FromDIP(wxSize(20, 20)), wxBORDER_NONE);
	button->SetBackgroundColour(wxColor(241, 248, 255));

	button->Bind(wxEVT_BUTTON, &TagElement::OnDelete, this);

	_mainSizer->Add(button, 0, wxALIGN_CENTER_VERTICAL);
}

void TagElement::OnDelete(wxCommandEvent& event) {
	if (OnDeleteTag) {
		OnDeleteTag(_tag);
	}
}