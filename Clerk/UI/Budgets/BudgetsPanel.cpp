#include "BudgetsPanel.h"

BudgetsPanel::BudgetsPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	model = new BudgetsListDataModel();
	list->AssociateModel(model.get());

	list->AppendTextColumn("Name", BudgetsListDataModel::ColumnName, wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("Period", BudgetsListDataModel::ColumnPeriod, wxDATAVIEW_CELL_INERT, 100, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);

	BudgetsProgressRender *render = new BudgetsProgressRender();

	wxDataViewColumn *column =	new wxDataViewColumn("Progress", render, BudgetsListDataModel::ColumnProgress, 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendColumn(column);

	list->AppendTextColumn("Limit", BudgetsListDataModel::ColumnLimit, wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("Current", BudgetsListDataModel::ColumnCurrent, wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("Remain", BudgetsListDataModel::ColumnRemain, wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("", BudgetsListDataModel::ColumnLast, wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &BudgetsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &BudgetsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Budget> BudgetsPanel::GetBudget() {	
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = (int)item.GetID() - 1;		
		return budgets[index];
	}	

	return nullptr;
}

void BudgetsPanel::Update() {
	budgets = DataHelper::GetInstance().GetBudgets();

	wxDateTime toDate = wxDateTime::Now();
	wxDateTime fromDate = wxDateTime::Now();	

	for (auto budget : budgets)
	{
		if (budget->period == Budget::Period::Week) {
			fromDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);
		}

		if (budget->period == Budget::Period::Month) {
			fromDate.SetDay(1);
		}

		if (budget->period == Budget::Period::Year) {
			fromDate.SetMonth(wxDateTime::Month::Jan);
			fromDate.SetDay(1);
		}

		budget->balance = DataHelper::GetInstance().GetExpensesForBudget(budget.get(), &fromDate, &toDate);
	}

	model.get()->SetItems(budgets);
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

void BudgetsPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	if (OnEdit) {
		auto budget = GetBudget();
		OnEdit(budget);
	}
}

void BudgetsPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(BudgetsPanelMenuTypes::Delete), wxT("Delete"));

	addItem->Enable(true);
	editItem->Enable(true);
	deleteItem->Enable(true);

	if (list->GetSelectedItemsCount() == 0) {
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

	list->PopupMenu(menu);
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