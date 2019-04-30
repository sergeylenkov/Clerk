#include "GoalsPanel.h"

GoalsPanel::GoalsPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);

	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &GoalsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_CONTEXT_MENU, &GoalsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Goal> GoalsPanel::GetGoal() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return goals[itemIndex];
	}

	return NULL;
}

void GoalsPanel::Update() {
	list->ClearAll();
	goals = DataHelper::GetInstance().GetGoals();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(200);

	list->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Amount"));
	column1.SetWidth(200);

	list->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Current"));
	column2.SetWidth(200);

	list->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Remain"));
	column3.SetWidth(200);

	list->InsertColumn(3, column3);

	int i = 0;

	for (auto goal : goals)
	{
		float currentAmount = DataHelper::GetInstance().GetBalanceForGoal(goal.get());

		float remainAmount = goal->amount - currentAmount;
		float remainPercent = currentAmount / (goal->amount / 100.0);

		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(goal->id);

		list->InsertItem(listItem);
		list->SetItem(i, 0, *goal->name);		
		list->SetItem(i, 1, wxString::Format("%.2f", goal->amount));
		list->SetItem(i, 2, wxString::Format("%.2f", currentAmount));
		list->SetItem(i, 3, wxString::Format("%.2f", goal->amount - currentAmount));

		i++;
	}
}

void GoalsPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void GoalsPanel::Edit() {
	if (OnEdit) {
		auto goal = GetGoal();

		OnEdit(goal);
	}
}

void GoalsPanel::Delete() {
	auto goal = GetGoal();

	if (goal) {
		goal->Delete();
		Update();
	}
}

void GoalsPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEdit) {
		auto goal = GetGoal();
		OnEdit(goal);
	}
}

void GoalsPanel::OnRightClick(wxContextMenuEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(GoalsPanelMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(GoalsPanelMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(GoalsPanelMenuTypes::Delete), wxT("Delete"));

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
	menu->Append(deleteItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &GoalsPanel::OnMenuSelect, this);

	wxPoint point = event.GetPosition();
	point = list->ScreenToClient(point);

	list->PopupMenu(menu, point);

	delete menu;
}

void GoalsPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<GoalsPanelMenuTypes>(event.GetId())) {
		case GoalsPanelMenuTypes::Add:
			Add();
			break;

		case GoalsPanelMenuTypes::Edit:
			Edit();
			break;

		case GoalsPanelMenuTypes::Delete:
			Delete();
			break;

		default:
			break;
	}
}