#include "AlertsListPanel.h"

AlertsListPanel::AlertsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_model = new AlertsListDataModel();
	_list->AssociateModel(_model.get());

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &AlertsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &AlertsListPanel::OnRightClick, this);

	_sortBy = AlertsListColumns::Name;
	_sortDesc = false;

	_alertsService = &_context.GetAlertsService();
	_subscriptionId = _alertsService->Subscribe([&]() {
		Update();
	});

	Update();
}

AlertsListPanel::~AlertsListPanel() {
	_alertsService->Unsubscribe(_subscriptionId);

	SaveColumnsSettings();
}

void AlertsListPanel::Update() {
	CreateListColumns();

	_alerts = _alertsService->GetAll();
	_model.get()->SetItems(_alerts);
}

std::shared_ptr<AlertPresentationModel> AlertsListPanel::GetAlert() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;

		return _alerts[index];
	}

	return nullptr;
}

void AlertsListPanel::CreateListColumns() {
	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetAlertsListColumns();

	for (auto& column : columns) {
		wxDataViewColumn* dataViewColumn = nullptr;

		switch (static_cast<AlertsListColumns>(column.index))
		{
			case AlertsListColumns::Name:
				dataViewColumn = _list->AppendTextColumn(_("Name"), static_cast<int>(AlertsListColumns::Name), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Message:
				dataViewColumn = _list->AppendTextColumn(_("Message"), static_cast<int>(AlertsListColumns::Message), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Type:
				dataViewColumn = _list->AppendTextColumn(_("Type"), static_cast<int>(AlertsListColumns::Type), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Period:
				dataViewColumn = _list->AppendTextColumn(_("Period"), static_cast<int>(AlertsListColumns::Period), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Condition:
				dataViewColumn = _list->AppendTextColumn(_("Condition"), static_cast<int>(AlertsListColumns::Condition), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Importance:
				dataViewColumn = _list->AppendTextColumn(_("Importance"), static_cast<int>(AlertsListColumns::Importance), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case AlertsListColumns::Amount:
				dataViewColumn = _list->AppendTextColumn(_("Amount"), static_cast<int>(AlertsListColumns::Amount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
		}

		if (column.sorted) {
			_sortBy = static_cast<AlertsListColumns>(column.index);
			_sortDesc = column.sortedDesc;

			if (dataViewColumn != nullptr) {
				dataViewColumn->SetSortOrder(!_sortDesc);
			}
		}
	}

	_list->AppendTextColumn("", static_cast<int>(AlertsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void AlertsListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetAlertsListColumns();

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = columns[i].index == static_cast<int>(_sortBy);
		columns[i].sortedDesc = _sortDesc;
	}

	Settings::GetInstance().SetAlertsListColumns(columns);
}

void AlertsListPanel::OnListItemDoubleClick(wxDataViewEvent& event) {
	auto alert = GetAlert();

	if (alert) {
		_context.GetCommandsInvoker().EditAlert(alert->id);
	}
	else {
		_context.GetCommandsInvoker().NewAlert();
	}
}

void AlertsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu* menu = new AlertContextMenu(_context.GetCommandsInvoker(), _icons, GetAlert());

	_list->PopupMenu(menu);

	delete menu;
}