#include "TagsPopup.h"

TagsPopup::TagsPopup(wxWindow *parent) : wxPopupWindow(parent) {
	panel = new wxScrolledWindow(this, wxID_ANY);
	panel->SetBackgroundColour(*wxLIGHT_GREY);
	
	list = new wxListCtrl(panel, wxID_ANY, wxPoint(0, 0), wxSize(200, 200), wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &TagsPopup::OnListItemDoubleClick, this);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(list, 0, wxALL, 1);

	panel->SetAutoLayout(true);
	panel->SetSizer(topSizer);

	topSizer->Fit(panel);

	SetClientSize(panel->GetSize());
}

TagsPopup::~TagsPopup() {
	//
}

void TagsPopup::Update(std::vector<std::shared_ptr<TagViewModel>> tags) {
	_tags = tags;
	list->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText(_(""));
	col.SetWidth(200);

	list->InsertColumn(0, col);

	for (unsigned int i = 0; i < tags.size(); i++) {
		wxListItem listItem;

		listItem.SetId(i);

		list->InsertItem(listItem);

		list->SetItem(i, 0, tags[i]->name);
	}

	list->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);	
}

void TagsPopup::SelectNext() {
	long index = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1 && index < list->GetItemCount() - 1) {
		index++;

		list->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		list->EnsureVisible(index);
	}
}

void TagsPopup::SelectPrev() {
	long index = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (index != -1 && index > 0) {
		index--;

		list->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		list->EnsureVisible(index);
	}
}

std::shared_ptr<TagViewModel> TagsPopup::GetSelectedTag() {
	long index = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

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