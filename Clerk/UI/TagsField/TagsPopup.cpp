#include "TagsPopup.h"

TagsPopup::TagsPopup(wxWindow *parent) : wxPopupWindow(parent) {
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_panel->SetBackgroundColour(*wxLIGHT_GREY);
	
	mainSizer->Add(_panel, 0, wxEXPAND, 0);

	_list = new wxListCtrl(_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	_list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &TagsPopup::OnListItemDoubleClick, this);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	verticalSizer->Add(_list, 0, wxEXPAND, 1);

	_panel->SetSizer(verticalSizer);
	_panel->Layout();

	SetSizer(mainSizer);
	Layout();
}

void TagsPopup::Position(wxPoint position, wxSize size) {
	wxPopupWindow::Position(position, size);

	SetSize(size);
	_list->SetMinSize(size);

	Layout();
}

void TagsPopup::Update(shared_vector<TagPresentationModel> tags) {
	_tags = tags;
	_list->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText("");
	col.SetWidth(_list->GetSize().GetWidth() - 40);
	
	_list->InsertColumn(0, col);

	for (unsigned int i = 0; i < tags.size(); i++) {
		wxListItem listItem;

		listItem.SetId(i);

		_list->InsertItem(listItem);

		_list->SetItem(i, 0, tags[i]->name);
	}

	_list->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);	
}

void TagsPopup::SelectNext() {
	long index = _list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1 && index < _list->GetItemCount() - 1) {
		index++;

		_list->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		_list->EnsureVisible(index);
	}
}

void TagsPopup::SelectPrev() {
	long index = _list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1 && index > 0) {
		index--;

		_list->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		_list->EnsureVisible(index);
	}
}

std::shared_ptr<TagPresentationModel> TagsPopup::GetSelectedTag() {
	if (_list->GetItemCount() == 0) {
		return nullptr;
	}

	long index = _list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1) {	
		return _tags[index];
	}

	return nullptr;
}

void TagsPopup::OnListItemDoubleClick(wxListEvent &event) {
	if (OnSelect) {
		OnSelect(GetSelectedTag());
	}
}