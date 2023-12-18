#include "SchedulersListPanel.h"

SchedulersListPanel::SchedulersListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_model = new SchedulersListDataModel();
	_list->AssociateModel(_model.get());	
	
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &SchedulersListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &SchedulersListPanel::OnRightClick, this);

	_sortBy = 2;
	_sortDesc = false;

	_schedulersService = &_context.GetSchedulersService();
	_subscriptionId = _schedulersService->Subscribe([&]() {
		Update();
	});

	Update();
}

SchedulersListPanel::~SchedulersListPanel() {
	_schedulersService->Unsubscribe(_subscriptionId);

	SaveColumnsSettings();
}

void SchedulersListPanel::Update() {
	CreateListColumns();

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
		switch (static_cast<SchedulersListDataModel::Columns>(column.index))
		{
		case SchedulersListDataModel::Columns::Name:
			_list->AppendTextColumn(_("Name"), static_cast<int>(SchedulersListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListDataModel::Columns::Type:
			_list->AppendTextColumn(_("Type"), static_cast<int>(SchedulersListDataModel::Columns::Type), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListDataModel::Columns::Amount:
			_list->AppendTextColumn(_("Amount"), static_cast<int>(SchedulersListDataModel::Columns::Amount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListDataModel::Columns::NextDate:
			_list->AppendTextColumn(_("Next Date"), static_cast<int>(SchedulersListDataModel::Columns::NextDate), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListDataModel::Columns::DaysLeft:
			_list->AppendTextColumn(_("Days Left"), static_cast<int>(SchedulersListDataModel::Columns::DaysLeft), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case SchedulersListDataModel::Columns::Status:
			_list->AppendTextColumn(_("Status"), static_cast<int>(SchedulersListDataModel::Columns::Status), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		}

		if (column.sorted) {
			_sortBy = column.index;
			_sortDesc = column.sortedDesc;
		}
	}

	_list->AppendTextColumn("", static_cast<int>(SchedulersListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void SchedulersListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetSchedulersListColumns();

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = i == _sortBy;
		columns[i].sortedDesc = _sortDesc;
	}

	Settings::GetInstance().SetSchedulersListColumns(columns);
}

void SchedulersListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	auto scheduler = GetScheduler();

	if (scheduler) {
		_context.GetCommandsInvoker().EditScheduler(scheduler->id);
	}
	else {
		_context.GetCommandsInvoker().NewScheduler();
	}
}

void SchedulersListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu* menu = new SchedulerContextMenu(_context.GetCommandsInvoker(), _icons, GetScheduler());

	_list->PopupMenu(menu);

	delete menu;
}