#include "TagsField.h"

TagsField::TagsField(wxWindow* parent, TagsService& tagsService, const wxPoint& position, const wxSize& size) : wxPanel(parent, wxID_ANY, position, size),
	_tagsService(tagsService)
{
	_popup = new TagsPopup(this);
	_popup->OnSelect = [&](std::shared_ptr<TagPresentationModel> tag) {
		AddTag(tag);
		_popup->Hide();
	};

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	_tagsSizer = new wxBoxSizer(wxHORIZONTAL);

	_textField = new wxTextCtrl(this, wxID_ANY);
	horizontalSizer->Add(_textField, 1, wxEXPAND);	

	mainSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, FromDIP(5));
	mainSizer->Add(_tagsSizer, 1, wxEXPAND);

	SetSizer(mainSizer);
	Layout();

	_textField->Bind(wxEVT_KEY_UP, &TagsField::OnTextChanged, this);
	_textField->Bind(wxEVT_CHAR_HOOK, &TagsField::OnTextKeyDown, this);
}

void TagsField::SetTags(shared_vector<TagPresentationModel> tags) {
	_tags = tags;

	Update();
}

shared_vector<TagPresentationModel> TagsField::GetTags() {
	return _tags;
}

void TagsField::Update() {
	_tagsSizer->Clear(true);

	for (auto& tag : _tags) {
		TagPanel* tagElement = new TagPanel(this);
		tagElement->SetTag(tag);

		tagElement->OnDelete = [&](std::shared_ptr<TagPresentationModel> tag) {
			DeleteTag(tag);
		};

		_tagsSizer->Add(tagElement, 0, wxRIGHT, 5);
	}
	
	Layout();
}

void TagsField::AddTag(std::shared_ptr<TagPresentationModel> tag) {	
	if (tag) {
		_tags.push_back(tag);
	}
	else {
		auto newTag = std::make_shared<TagPresentationModel>();
		newTag->name = _textField->GetValue().Trim(true).Trim(false);

		_tags.push_back(newTag);
	}

	_textField->SetValue("");

	Update();

	if (OnChange) {
		OnChange();
	}
}

void TagsField::DeleteTag(std::shared_ptr<TagPresentationModel> tag) {
	auto end = std::remove_if(_tags.begin(), _tags.end(), [tag](auto el) {
		return tag->id == el->id;
	});

	_tags.erase(end, _tags.end());

	Update();

	if (OnChange) {
		OnChange();
	}
}

void TagsField::OnTextChanged(wxKeyEvent& event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		_popup->Hide();
	}
	else if (event.GetKeyCode() == WXK_UP) {
		_popup->SelectPrev();
		event.StopPropagation();
	}
	else if (event.GetKeyCode() == WXK_DOWN) {
		_popup->SelectNext();
	}
	else if (event.GetKeyCode() == WXK_RETURN) {
		AddTag(_popup->GetSelectedTag());
		_popup->Hide();
	}
	else {
		wxString value = _textField->GetValue().Trim(true).Trim(false);

		if (!value.empty()) {
			auto tags = _tagsService.GetBySearch(value);

			if (tags.size() > 0) {
				wxPoint position = _textField->GetScreenPosition();
				wxSize size = _textField->GetSize();
				wxSize panelSize = wxSize(size.GetWidth(), FromDIP(300));

				_popup->Position(wxPoint(position.x - panelSize.GetWidth(), (position.y - panelSize.GetHeight()) + size.GetHeight()), panelSize);
				_popup->Update(tags);
				_popup->Show();
			}
			else {
				_popup->Hide();
			}
		}
	}

	event.Skip();
}

void TagsField::OnTextKeyDown(wxKeyEvent& event) {
	if ((int)event.GetKeyCode() == WXK_ESCAPE && _popup->IsShown()) {
		event.StopPropagation();
	}
	else {
		event.Skip();
	}
}