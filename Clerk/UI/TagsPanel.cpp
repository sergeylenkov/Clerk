#include "TagsPanel.h"

TagsPanel::TagsPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons) {
	editedIndex = 0;

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *topPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(topPanel, wxID_ANY, _("Search:"), wxDefaultPosition, wxDefaultSize, 0);	
	boxSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	searchField = new wxTextCtrl(topPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0);
	searchField->Bind(wxEVT_TEXT, &TagsPanel::OnSearchChanged, this);

	boxSizer->Add(searchField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	topPanel->SetSizer(boxSizer);
	topPanel->Layout();

	boxSizer->Fit(topPanel);
	mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);

	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxLC_EDIT_LABELS | wxBORDER_NONE);
	list->Bind(wxEVT_LIST_BEGIN_LABEL_EDIT, &TagsPanel::OnListItemBeginEdit, this);
	list->Bind(wxEVT_LIST_END_LABEL_EDIT, &TagsPanel::OnListItemEndEdit, this);
	list->Bind(wxEVT_CONTEXT_MENU, &TagsPanel::OnRightClick, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &TagsPanel::OnListItemDoubleClick, this);

	mainSizer->Add(list, 1, wxEXPAND | wxALL, 5);

	SetSizer(mainSizer);
	Layout();
}

TagsPanel::~TagsPanel() {
	
}

void TagsPanel::Update() {
	std::thread([=]()
	{
		tags = _context.GetTagsService().GetAll();
		Filter();

		GetEventHandler()->CallAfter(&TagsPanel::UpdateList);
	}).detach();
}

void TagsPanel::Filter() {
	filteredTags.clear();

	if (searchField->GetValue().Length() > 0) {
		wxString search = searchField->GetValue();				

		for (auto& tag : tags)
		{
			if (String::Search(tag->name, search)) {
				filteredTags.push_back(tag);
			}
		}
	}
	else {
		filteredTags = tags;
	}
}

void TagsPanel::UpdateList() {
	editedIndex = -1;

	list->ClearAll();

	int i = 0;

	for (auto &tag : filteredTags)
	{
		list->InsertItem(i, wxString::Format("%s (%d)", tag->name, tag->count));
		i++;
	}
}

std::shared_ptr<TagPresentationModel> TagsPanel::GetTag() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return filteredTags[itemIndex];
	}

	return nullptr;
}

long TagsPanel::GetSelectedIndex() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return itemIndex;
	}

	return itemIndex;
}

void TagsPanel::DeleteItemAtIndex(long index) {
	auto &tag = filteredTags[index];

	for (unsigned int i = 0; i < tags.size(); i++) {
		if (tag->id == tags[i]->id) {
			tags.erase(tags.begin() + i);
			break;
		}
	}

	list->DeleteItem(index);
	filteredTags.erase(filteredTags.begin() + index);
}

void TagsPanel::ShowTransactions() {

}

void TagsPanel::Rename() {
	long index = GetSelectedIndex();

	if (index != -1) {
		editedIndex = index;

		auto &tag = filteredTags[index];

		list->SetItemText(index, tag->name);
		list->EditLabel(index);
	}
}

void TagsPanel::Delete() {
	auto tag = GetTag();

	if (tag) {
		wxMessageDialog *dialog = new wxMessageDialog(this, wxString::Format(_("Delete tag confirm"), tag->name), _("Delete tag"), wxOK | wxCANCEL | wxCENTER);

		if (dialog->ShowModal() == wxID_OK) {
			//TODO
			//tag->Delete();

			long index = GetSelectedIndex();

			if (index != -1) {
				DeleteItemAtIndex(index);
			}
		}
	}
}

void TagsPanel::OnSearchChanged(wxCommandEvent &event) {
	Filter();
	UpdateList();
}

void TagsPanel::OnListItemBeginEdit(wxListEvent &event) {
	if (editedIndex == -1) {
		event.Veto();
	}
}

void TagsPanel::OnListItemEndEdit(wxListEvent &event) {
	if (editedIndex != -1) {
		wxString newValue = event.GetItem().GetText();
		newValue = newValue.Trim(true).Trim(false);

		auto &editedTag = filteredTags[editedIndex];

		if (newValue != editedTag->name) {
			bool isReplaced = false;

			for (unsigned int i = 0; i < tags.size(); i++)
			{
				auto &tag = tags[i];

				if (newValue == tag->name) {
					//DataHelper::GetInstance().ReplaceTag(editedTag->id, tag->id);
					//TODO
					//editedTag->Delete();

					tag->count = tag->count + editedTag->count;
					list->SetItemText(i, wxString::Format("%s (%d)", tag->name, tag->count));

					isReplaced = true;
				}
			}

			if (isReplaced) {	
				DeleteItemAtIndex(editedIndex);
			} else {
				editedTag->name = wxString(newValue);
				//TODO
				//editedTag->Save();

				list->SetItemText(editedIndex, wxString::Format("%s (%d)", editedTag->name, editedTag->count));
			}
		}
		else {
			list->SetItemText(editedIndex, wxString::Format("%s (%d)", editedTag->name, editedTag->count));
		}

		editedIndex = -1;

		event.Veto();
		list->EndEditLabel(false);
	}
}

void TagsPanel::OnListItemDoubleClick(wxListEvent &event) {
	Rename();
}

void TagsPanel::OnRightClick(wxContextMenuEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(static_cast<int>(TagsPanelMenuTypes::Show), _("Show transactions"));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(TagsPanelMenuTypes::Rename), _("Rename"));
	menu->Append(static_cast<int>(TagsPanelMenuTypes::Delete), _("Delete..."));

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TagsPanel::OnMenuSelect, this);

	wxPoint point = event.GetPosition();
	point = list->ScreenToClient(point);

	list->PopupMenu(menu, point);

	delete menu;
}

void TagsPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<TagsPanelMenuTypes>(event.GetId())) {
		case TagsPanelMenuTypes::Show:
			ShowTransactions();
			break;

		case TagsPanelMenuTypes::Rename:
			Rename();
			break;

		case TagsPanelMenuTypes::Delete:
			Delete();
			break;

		default:
			break;
	}
}