#include "SchedulersPanel.h"

SchedulersPanel::SchedulersPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	list->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &SchedulersPanel::OnListItemClick, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &SchedulersPanel::OnListItemDoubleClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Scheduler> SchedulersPanel::GetScheduler() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return schedulers[itemIndex];
	}

	return NULL;
}

void SchedulersPanel::Update() {
	list->ClearAll();
	schedulers = DataHelper::GetInstance().GetSchedulers();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(200);

	list->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Type"));
	column1.SetWidth(200);

	list->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Previous Date"));
	column2.SetWidth(200);

	list->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Next Date"));
	column3.SetWidth(200);

	list->InsertColumn(3, column3);

	wxListItem column4;

	column4.SetId(4);
	column4.SetText(_("Status"));
	column4.SetWidth(200);

	list->InsertColumn(4, column4);

	int i = 0;

	for each (auto scheduler in schedulers)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(scheduler->id);

		list->InsertItem(listItem);
		list->SetItem(i, 0, *scheduler->name);

		wxString type("");

		if (scheduler->type == SchedulerTypes::Daily) {
			type = "Daily";
		}

		if (scheduler->type == SchedulerTypes::Weekly) {
			type = "Weekly";
		}

		if (scheduler->type == SchedulerTypes::Monthly) {
			type = "Monthly";
		}

		if (scheduler->type == SchedulerTypes::Yearly) {
			type = "Yearly";
		}

		list->SetItem(i, 1, type);

		list->SetItem(i, 2, scheduler->previousDate->Format("%B %d"));
		list->SetItem(i, 3, scheduler->nextDate->Format("%B %d"));

		if (scheduler->active) {
			list->SetItem(i, 4, "Running");
		}
		else {
			list->SetItem(i, 4, "Paused");
		}
	}
}

void SchedulersPanel::EditScheduler() {
	if (OnEditScheduler) {
		auto scheduler = GetScheduler();

		OnEditScheduler(scheduler);
	}
}

void SchedulersPanel::DeleteScheduler() {
	auto scheduler = GetScheduler();

	if (scheduler) {
		scheduler->Delete();
		Update();
	}
}

void SchedulersPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(static_cast<int>(SchedulersPanelMenuTypes::Edit), wxT("Edit..."));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(SchedulersPanelMenuTypes::Delete), wxT("Delete..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &SchedulersPanel::OnMenuSelect, this);

	list->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void SchedulersPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEditScheduler) {
		auto budget = GetScheduler();
		OnEditScheduler(budget);
	}
}

void SchedulersPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<SchedulersPanelMenuTypes>(event.GetId())) {
	case SchedulersPanelMenuTypes::Edit:
		EditScheduler();
		break;

	case SchedulersPanelMenuTypes::Delete:
		DeleteScheduler();
		break;

	default:
		break;
	}
}