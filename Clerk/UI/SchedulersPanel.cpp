#include "SchedulersPanel.h"

SchedulersPanel::SchedulersPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &SchedulersPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_CONTEXT_MENU, &SchedulersPanel::OnRightClick, this);
	
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

	return nullptr;
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
	column1.SetWidth(120);

	list->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Previous Date"));
	column2.SetWidth(120);

	list->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Next Date"));
	column3.SetWidth(120);

	list->InsertColumn(3, column3);

	wxListItem column4;

	column4.SetId(4);
	column4.SetText(_("Status"));
	column4.SetWidth(100);
	column4.SetAlign(wxLIST_FORMAT_CENTER);

	list->InsertColumn(4, column4);

	int i = 0;

	for each (auto scheduler in schedulers)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(scheduler->id);

		if (!scheduler->active) {
			listItem.SetMask(wxLIST_MASK_TEXT);
			listItem.SetTextColour(wxColour(100, 100, 100));
		}

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

		i++;
	}
}

void SchedulersPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void SchedulersPanel::Edit() {
	if (OnEdit) {
		auto scheduler = GetScheduler();
		OnEdit(scheduler);
	}
}

void SchedulersPanel::Delete() {
	auto scheduler = GetScheduler();

	if (scheduler) {
		scheduler->Delete();
		Update();
	}
}

void SchedulersPanel::Run() {
	auto scheduler = GetScheduler();

	scheduler->Run();
	scheduler->Save();

	Update();
}

void SchedulersPanel::Pause() {
	auto scheduler = GetScheduler();

	scheduler->active = false;
	scheduler->Save();

	Update();
}

void SchedulersPanel::OnListItemDoubleClick(wxListEvent &event) {
	Edit();
}

void SchedulersPanel::OnRightClick(wxContextMenuEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(SchedulersPanelMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(SchedulersPanelMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(SchedulersPanelMenuTypes::Delete), wxT("Delete"));

	addItem->Enable(true);
	editItem->Enable(true);
	deleteItem->Enable(true);

	if (list->GetSelectedItemCount() == 0) {
		editItem->Enable(false);
		editItem->SetTextColour(*wxLIGHT_GREY);

		deleteItem->Enable(false);
		deleteItem->SetTextColour(*wxLIGHT_GREY);
	}

	menu->Append(addItem);
	menu->Append(editItem);
	menu->AppendSeparator();

	if (list->GetSelectedItemCount() > 0) {
		auto scheduler = GetScheduler();

		if (scheduler->active) {
			wxMenuItem *pauseItem = new wxMenuItem(menu, static_cast<int>(SchedulersPanelMenuTypes::Pause), wxT("Pause"));
			menu->Append(pauseItem);
		}
		else {
			wxMenuItem *runItem = new wxMenuItem(menu, static_cast<int>(SchedulersPanelMenuTypes::Run), wxT("Run"));
			menu->Append(runItem);
		}

		menu->AppendSeparator();
	}

	menu->Append(deleteItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &SchedulersPanel::OnMenuSelect, this);

	wxPoint point = event.GetPosition();
	point = list->ScreenToClient(point);

	list->PopupMenu(menu, point);
	
	delete menu;
}

void SchedulersPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<SchedulersPanelMenuTypes>(event.GetId())) {
		case SchedulersPanelMenuTypes::Add:
			Add();
			break;

		case SchedulersPanelMenuTypes::Edit:
			Edit();
			break;

		case SchedulersPanelMenuTypes::Delete:
			Delete();
			break;

		case SchedulersPanelMenuTypes::Run:
			Run();
			break;

		case SchedulersPanelMenuTypes::Pause:
			Pause();
			break;

		default:
			break;
	}
}