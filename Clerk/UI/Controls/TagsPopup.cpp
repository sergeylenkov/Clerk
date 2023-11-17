#include "TagsPopup.h"

TagsPopup::TagsPopup(wxWindow *parent) : wxPopupWindow(parent) {
	_panel = new wxScrolledWindow(this, wxID_ANY);
	_panel->SetBackgroundColour(*wxLIGHT_GREY);
	
	_list = new wxListCtrl(_panel, wxID_ANY, wxPoint(0, 0), this->FromDIP(wxSize(200, 200)), wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	_list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &TagsPopup::OnListItemDoubleClick, this);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(_list, 0, wxALL, 1);

	_panel->SetAutoLayout(true);
	_panel->SetSizer(topSizer);

	topSizer->Fit(_panel);

	SetClientSize(_panel->GetSize());
}

TagsPopup::~TagsPopup() {
	//
}

void TagsPopup::Update(std::vector<std::shared_ptr<TagPresentationModel>> tags) {
	_tags = tags;
	_list->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText(_(""));
	col.SetWidth(200);

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
	long index = _list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1) {	
		return _tags[index];
	}

	return nullptr;
}

void TagsPopup::OnListItemDoubleClick(wxListEvent &event) {
	if (OnSelectTag) {
		OnSelectTag();
	}
}