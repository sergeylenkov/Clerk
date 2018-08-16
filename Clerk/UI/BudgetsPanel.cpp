#include "BudgetsPanel.h"

BudgetsPanel::BudgetsPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	budgetsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	budgetsList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &BudgetsPanel::OnListItemClick, this);
	budgetsList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &BudgetsPanel::OnListItemDoubleClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(budgetsList, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Budget> BudgetsPanel::GetBudget() {
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

void BudgetsPanel::Update() {
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

void BudgetsPanel::EditBudget() {	
	if (OnEditBudget) {
		auto budget = GetBudget();

		OnEditBudget(budget);
	}
}

void BudgetsPanel::DeleteBudget() {
	auto budget = GetBudget();

	if (budget) {
		budget->Delete();
		Update();
	}
}

void BudgetsPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(static_cast<int>(BudgetsPanelMenuTypes::Edit), wxT("Edit..."));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(BudgetsPanelMenuTypes::Delete), wxT("Delete..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &BudgetsPanel::OnMenuSelect, this);

	budgetsList->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void BudgetsPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEditBudget) {
		auto budget = GetBudget();
		OnEditBudget(budget);
	}
}

void BudgetsPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<BudgetsPanelMenuTypes>(event.GetId())) {
		case BudgetsPanelMenuTypes::Edit:
				EditBudget();
				break;

		case BudgetsPanelMenuTypes::Delete:
				DeleteBudget();
				break;

		default:
			break;
	}
}