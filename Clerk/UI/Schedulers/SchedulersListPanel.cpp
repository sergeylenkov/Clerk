#include "SchedulersListPanel.h"

SchedulersListPanel::SchedulersListPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	list->AppendTextColumn(_("Name"), static_cast<int>(SchedulersListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Type"), static_cast<int>(SchedulersListDataModel::Columns::Type), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Amount"), static_cast<int>(SchedulersListDataModel::Columns::Amount), wxDATAVIEW_CELL_INERT, 100, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Next Date"), static_cast<int>(SchedulersListDataModel::Columns::NextDate), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Days Left"), static_cast<int>(SchedulersListDataModel::Columns::DaysLeft), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn(_("Status"), static_cast<int>(SchedulersListDataModel::Columns::Status), wxDATAVIEW_CELL_INERT, 100, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	list->AppendTextColumn("", static_cast<int>(SchedulersListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	model = new SchedulersListDataModel();
	list->AssociateModel(model.get());	
	
	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &SchedulersListPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &SchedulersListPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

std::shared_ptr<SchedulerPresentationModel> SchedulersListPanel::GetScheduler() {
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = reinterpret_cast<int>(item.GetID()) - 1;
		return schedulers[index];
	}

	return nullptr;
}

void SchedulersListPanel::Update() {
	schedulers = _context.GetSchedulersService().GetAll();
	model.get()->SetItems(schedulers);
}

void SchedulersListPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void SchedulersListPanel::Edit() {
	if (OnEdit) {
		auto scheduler = GetScheduler();
		OnEdit(scheduler);
	}
}

void SchedulersListPanel::Delete() {
	auto scheduler = GetScheduler();

	if (scheduler) {		
		//_context.GetSchedulersRepository().Delete(*scheduler);
		Update();
	}
}

void SchedulersListPanel::Run() {
	auto scheduler = GetScheduler();

	//TODO moved method to interactor
	//scheduler->Run();
	//scheduler->Save();

	Update();
}

void SchedulersListPanel::Pause() {
	auto scheduler = GetScheduler();

	//TODO moved method to interactor
	//scheduler->active = false;
	//scheduler->Save();

	Update();
}

void SchedulersListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	Edit();
}

void SchedulersListPanel::OnRightClick(wxDataViewEvent &event) {
	/*wxMenu* menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(SchedulerContextMenuTypes::Add), _("Add..."));
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

	if (list->GetSelectedItemsCount() > 0) {
		auto scheduler = GetScheduler();

		if (scheduler->isActive) {
			wxMenuItem *pauseItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Pause), _("Pause"));
			menu->Append(pauseItem);
		}
		else {
			wxMenuItem *runItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Run), _("Run"));
			menu->Append(runItem);
		}

		menu->AppendSeparator();
	}

	menu->Append(deleteItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &SchedulersListPanel::OnMenuSelect, this);

	list->PopupMenu(menu);
	
	delete menu;*/
}

void SchedulersListPanel::OnMenuSelect(wxCommandEvent &event) {
	/*switch (static_cast<ContextMenuTypes>(event.GetId())) {
		case ContextMenuTypes::Add:
			Add();
			break;

		case ContextMenuTypes::Edit:
			Edit();
			break;

		case ContextMenuTypes::Delete:
			Delete();
			break;

		case ContextMenuTypes::Run:
			Run();
			break;

		case ContextMenuTypes::Pause:
			Pause();
			break;

		default:
			break;
	}*/
}