#include "SchedulersListPanel.h"

SchedulersListPanel::SchedulersListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataListPanel(parent, context, icons)
{
	_model = new SchedulersListDataModel();
	_list->AssociateModel(_model.get());	

	_schedulersService = &_context.GetSchedulersService();
	_subscriptionId = _schedulersService->Subscribe([&]() {
		Update();
	});

	CreateListColumns();
	Update();
}

SchedulersListPanel::~SchedulersListPanel() {
	_schedulersService->Unsubscribe(_subscriptionId);
	SaveColumnsSettings();
}

void SchedulersListPanel::Update() {
	_schedulers = _schedulersService->GetAll();
	_model.get()->SetItems(_schedulers);
}

std::shared_ptr<SchedulerPresentationModel> SchedulersListPanel::GetScheduler() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;
		return _schedulers[index];
	}

	return nullptr;
}

void SchedulersListPanel::CreateListColumns() {
	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetSchedulersListColumns();
	
	for (auto& column : columns) {
		wxDataViewColumn* dataViewColumn = nullptr;

		switch (static_cast<SchedulersListColumns>(column.index))
		{
		case SchedulersListColumns::Name:
			dataViewColumn = _list->AppendTextColumn(_("Name"), static_cast<int>(SchedulersListColumns::Name), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListColumns::Type:
			dataViewColumn = _list->AppendTextColumn(_("Type"), static_cast<int>(SchedulersListColumns::Type), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListColumns::Amount:
			dataViewColumn = _list->AppendTextColumn(_("Amount"), static_cast<int>(SchedulersListColumns::Amount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListColumns::NextDate:
			dataViewColumn = _list->AppendTextColumn(_("Next Date"), static_cast<int>(SchedulersListColumns::NextDate), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListColumns::DaysLeft:
			dataViewColumn = _list->AppendTextColumn(_("Days Left"), static_cast<int>(SchedulersListColumns::DaysLeft), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListColumns::Status:
			dataViewColumn = _list->AppendTextColumn(_("Status"), static_cast<int>(SchedulersListColumns::Status), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		}
	}
	
	_list->AppendTextColumn("", static_cast<int>(SchedulersListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	RestoreSorting(columns);
}

void SchedulersListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetSchedulersListColumns();
	columns = UpdateColumnsSettings(columns);

	Settings::GetInstance().SetSchedulersListColumns(columns);
}

void SchedulersListPanel::EditSelectedItem() {
	auto scheduler = GetScheduler();

	if (scheduler) {
		_context.GetCommandsInvoker().EditScheduler(scheduler->id);
	}
	else {
		_context.GetCommandsInvoker().NewScheduler();
	}
}

void SchedulersListPanel::ShowContextMenu() {
	wxMenu* menu = new SchedulerContextMenu(_context.GetCommandsInvoker(), _icons, GetScheduler());

	_list->PopupMenu(menu);

	delete menu;
}