#include "GoalsPanel.h"

GoalsPanel::GoalsPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	model = new GoalsListDataModel();
	list->AssociateModel(model.get());

	list->AppendTextColumn(_("Name"), static_cast<int>(GoalsListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Due Date"), static_cast<int>(GoalsListDataModel::Columns::DueDate), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Days Remain"), static_cast<int>(GoalsListDataModel::Columns::DaysRemain), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);

	GoalsProgressRender *render = new GoalsProgressRender();

	wxDataViewColumn *column = new wxDataViewColumn(_("Progress"), render, static_cast<int>(GoalsListDataModel::Columns::Progress), 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendColumn(column);

	list->AppendTextColumn(_("Goal"), static_cast<int>(GoalsListDataModel::Columns::Goal), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Current"), static_cast<int>(GoalsListDataModel::Columns::Current), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Remain"), static_cast<int>(GoalsListDataModel::Columns::Remain), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("", static_cast<int>(GoalsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &GoalsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &GoalsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

std::shared_ptr<GoalPresentationModel> GoalsPanel::GetGoal() {
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = reinterpret_cast<int>(item.GetID()) - 1;
		return goals[index];
	}

	return nullptr;	
}

void GoalsPanel::Update() {
	goals = _context.GetGoalsService().GetAll();

	model.get()->SetItems(goals);
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
		//_context.GetGoalsRepository().Delete(*goal);
		Update();
	}
}

void GoalsPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	if (OnEdit) {
		auto goal = GetGoal();
		OnEdit(goal);
	}
}

void GoalsPanel::OnRightClick(wxDataViewEvent &event) {
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

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &GoalsPanel::OnMenuSelect, this);
	
	list->PopupMenu(menu);
}

void GoalsPanel::OnMenuSelect(wxCommandEvent &event) {
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