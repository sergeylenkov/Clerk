#include "AlertsPanel.h"

AlertsPanel::AlertsPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	model = new AlertsListDataModel();
	list->AssociateModel(model.get());

	list->AppendTextColumn(_("Name"), static_cast<int>(AlertsListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Type"), static_cast<int>(AlertsListDataModel::Columns::Type), wxDATAVIEW_CELL_INERT, 150, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Period"), static_cast<int>(AlertsListDataModel::Columns::Period), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Condition"), static_cast<int>(AlertsListDataModel::Columns::Condition), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Amount"), static_cast<int>(AlertsListDataModel::Columns::Amount), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("", static_cast<int>(AlertsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &AlertsPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &AlertsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

std::shared_ptr<AlertPresentationModel> AlertsPanel::GetAlert() {
	wxDataViewItem item = list->GetSelection();
	
	if (item.IsOk()) {
		int index = reinterpret_cast<int>(item.GetID()) - 1;

		return alerts[index];
	}	

	return nullptr;
}

void AlertsPanel::Update() {
	alerts = _context.GetAlertsService().GetAll();
	model.get()->SetItems(alerts);
}

void AlertsPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void AlertsPanel::Edit() {
	if (OnEdit) {
		auto alert = GetAlert();
		OnEdit(alert);
	}
}

void AlertsPanel::Delete() {
	auto alert = GetAlert();

	if (alert) {
		//_context.GetAlertsRepository().Delete(*alert);
		Update();
	}
}

void AlertsPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	if (OnEdit) {
		auto alert = GetAlert();
		OnEdit(alert);
	}
}

void AlertsPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Delete), wxT("Delete"));

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

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &AlertsPanel::OnMenuSelect, this);

	list->PopupMenu(menu);
}

void AlertsPanel::OnMenuSelect(wxCommandEvent &event) {
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