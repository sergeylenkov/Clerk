#include "BudgetsListPanel.h"

BudgetsListPanel::BudgetsListPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	budgetsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	budgetsList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &BudgetsListPanel::OnListItemClick, this);
	budgetsList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &BudgetsListPanel::OnListItemDoubleClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(budgetsList, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Budget> BudgetsListPanel::GetBudget() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = budgetsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return budgets[itemIndex];
	}

	return NULL;
}

void BudgetsListPanel::Update() {
	budgetsList->ClearAll();	
	budgets = DataHelper::GetInstance().GetBudgets();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(200);

	budgetsList->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Current"));
	column1.SetWidth(200);

	budgetsList->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Limit"));
	column2.SetWidth(200);

	budgetsList->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Remain"));
	column3.SetWidth(200);

	budgetsList->InsertColumn(3, column3);

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

		budgetsList->InsertItem(listItem);
		budgetsList->SetItem(i, 0, *budget->name);
		budgetsList->SetItem(i, 1, wxString::Format("%.2f", currentAmount));
		budgetsList->SetItem(i, 2, wxString::Format("%.2f", budget->amount));
		budgetsList->SetItem(i, 3, wxString::Format("%.2f", budget->amount - currentAmount));
		
		i++;
	}
}

void BudgetsListPanel::EditBudget() {
	if (OnEditBudget) {
		OnEditBudget();
	}
}

void BudgetsListPanel::DeleteBudget() {
	auto budget = GetBudget();

	if (budget) {
		budget->Delete();
		Update();
	}
}

void BudgetsListPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(ID_EditBudget, wxT("Edit..."));
	menu->AppendSeparator();
	menu->Append(ID_DeleteBudget, wxT("Delete..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &BudgetsListPanel::OnMenuSelect, this);

	budgetsList->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void BudgetsListPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEditBudget) {
		OnEditBudget();
	}
}

void BudgetsListPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (event.GetId()) {
		case ID_EditBudget:
			EditBudget();
			break;

		case ID_DeleteBudget:
			DeleteBudget();
			break;

		default:
			break;
	}
}