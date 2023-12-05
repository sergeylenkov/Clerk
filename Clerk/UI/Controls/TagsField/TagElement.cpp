#include "TagElement.h"

TagElement::TagElement(wxWindow* parent, const wxPoint& pos, const wxSize& size) : wxPanel(parent, wxID_ANY, pos, size)
{
	SetBackgroundColour(wxColor(241, 248, 255));

	_mainSizer = new wxBoxSizer(wxHORIZONTAL);

	this->SetSizer(_mainSizer);
	this->Layout();
}

TagElement::~TagElement()
{
	//
}

void TagElement::SetTag(std::shared_ptr<TagPresentationModel> tag) {
	_tag = tag;

	_mainSizer->Clear();

	wxStaticText* label = new wxStaticText(this, wxID_ANY, tag->name);
	_mainSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxButton* button = new wxButton(this, wxID_ANY, "x", wxDefaultPosition, wxSize(40, 40), wxBORDER_NONE);
	button->Bind(wxEVT_BUTTON, &TagElement::OnDelete, this);

	_mainSizer->Add(button, 0, wxRIGHT, 5);
}

void TagElement::OnDelete(wxCommandEvent& event) {
	if (OnDeleteTag) {
		OnDeleteTag(_tag);
	}
}