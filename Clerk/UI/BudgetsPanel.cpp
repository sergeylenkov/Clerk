#include "BudgetsPanel.h"

BudgetsPanel::BudgetsPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &BudgetsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_CONTEXT_MENU, &BudgetsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Budget> BudgetsPanel::GetBudget() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return budgets[itemIndex];
	}

	return NULL;
}

void BudgetsPanel::Update() {
	list->ClearAll();	
	budgets = DataHelper::GetInstance().GetBudgets();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(200);

	list->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Current"));
	column1.SetWidth(200);

	list->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Limit"));
	column2.SetWidth(200);

	list->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Remain"));
	column3.SetWidth(200);

	list->InsertColumn(3, column3);

	int i = 0;

	for each (auto budget in budgets)
	{
		float currentAmount = 0.0;
		wxDateTime toDate = wxDateTime::Now();
		wxDateTime fromDate = wxDateTime::Now();

		if (budget->period == BudgetPeriods::Month) {
			fromDate.SetDay(1);
		}
		
		if (budget->type == BudgetTypes::Limit) {
			if (!budget->account) {
				currentAmount = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
			}
			else {
				currentAmount = DataHelper::GetInstance().GetExpensesSumForAccount(budget->account.get(), &fromDate, &toDate);
			}
		}

		float remainAmount = budget->amount - currentAmount;
		float remainPercent = currentAmount / (budget->amount / 100.0);

		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(budget->id);

		if (remainAmount < 0 || remainPercent < 10) {
			listItem.SetMask(wxLIST_MASK_TEXT);
			listItem.SetTextColour(wxColour(255, 0, 0));
		}

		list->InsertItem(listItem);
		list->SetItem(i, 0, *budget->name);
		list->SetItem(i, 1, wxString::Format("%.2f", currentAmount));
		list->SetItem(i, 2, wxString::Format("%.2f", budget->amount));
		list->SetItem(i, 3, wxString::Format("%.2f", budget->amount - currentAmount));
		
		i++;
	}
}

void BudgetsPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void BudgetsPanel::Edit() {	
	if (OnEdit) {
		auto budget = GetBudget();

		OnEdit(budget);
	}
}

void BudgetsPanel::Delete() {
	auto budget = GetBudget();

	if (budget) {
		budget->Delete();
		Update();
	}
}

void BudgetsPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEdit) {
		auto budget = GetBudget();
		OnEdit(budget);
	}
}

void BudgetsPanel::OnRightClick(wxContextMenuEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Delete), wxT("Delete..."));

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

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &BudgetsPanel::OnMenuSelect, this);

	wxPoint point = event.GetPosition();
	point = ScreenToClient(point);

	list->PopupMenu(menu, point);

	delete menu;
}

void BudgetsPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<BudgetsPanelMenuTypes>(event.GetId())) {
		case BudgetsPanelMenuTypes::Add:
			Add();
			break;

		case BudgetsPanelMenuTypes::Edit:
			Edit();
			break;

		case BudgetsPanelMenuTypes::Delete:
			Delete();
			break;

		default:
			break;
	}
}