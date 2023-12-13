#include "BudgetsPanel.h"

BudgetsPanel::BudgetsPanel(wxWindow *parent, DataContext& context): DataPanel(parent, context) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	model = new BudgetsListDataModel();
	list->AssociateModel(model.get());

	list->AppendTextColumn(_("Name"), static_cast<int>(BudgetsListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Period"), static_cast<int>(BudgetsListDataModel::Columns::Period), wxDATAVIEW_CELL_INERT, 100, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);

	BudgetsProgressRender *render = new BudgetsProgressRender();

	wxDataViewColumn *column =	new wxDataViewColumn(_("Progress"), render, static_cast<int>(BudgetsListDataModel::Columns::Progress), 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendColumn(column);

	list->AppendTextColumn(_("Limit"), static_cast<int>(BudgetsListDataModel::Columns::Limit), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Current"), static_cast<int>(BudgetsListDataModel::Columns::Current), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Remain"), static_cast<int>(BudgetsListDataModel::Columns::Remain), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("", static_cast<int>(BudgetsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &BudgetsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &BudgetsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

std::shared_ptr<BudgetPresentationModel> BudgetsPanel::GetBudget() {	
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;
		return _budgets[index];
	}	

	return nullptr;
}

void BudgetsPanel::Update() {
	_budgets = _context.GetBudgetsService().GetAll();
	model.get()->SetItems(_budgets);
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
		//_context.GetBudgetsRepository().Delete(*budget);
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

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Add), _("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Edit), _("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Delete), _("Delete"));

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
	switch (static_cast<ContextMenuTypes>(event.GetId())) {
		case ContextMenuTypes::Add:
			Add();
			break;

		case ContextMenuTypes::Edit:
			Edit();
			break;

		case ContextMenuTypes::Delete:
			Delete();
			break;

		default:
			break;
	}
}