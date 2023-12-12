#include "AlertsPanel.h"

AlertsPanel::AlertsPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_model = new AlertsListDataModel();
	_list->AssociateModel(_model.get());

	_list->AppendTextColumn(_("Name"), static_cast<int>(AlertsListDataModel::Columns::Name), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Message"), static_cast<int>(AlertsListDataModel::Columns::Message), wxDATAVIEW_CELL_INERT, 300, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Type"), static_cast<int>(AlertsListDataModel::Columns::Type), wxDATAVIEW_CELL_INERT, 150, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Period"), static_cast<int>(AlertsListDataModel::Columns::Period), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Condition"), static_cast<int>(AlertsListDataModel::Columns::Condition), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Importance"), static_cast<int>(AlertsListDataModel::Columns::Importance), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn(_("Amount"), static_cast<int>(AlertsListDataModel::Columns::Amount), wxDATAVIEW_CELL_INERT, 150, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
	_list->AppendTextColumn("", static_cast<int>(AlertsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &AlertsPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &AlertsPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_alertsService = &_context.GetAlertsService();
	_subscriptionId = _alertsService->Subscribe([&]() {
		Update();
	});

	Update();
}

AlertsPanel::~AlertsPanel() {
	_alertsService->Unsubscribe(_subscriptionId);
}

void AlertsPanel::Update() {
	_alerts = _alertsService->GetAll();
	_model.get()->SetItems(_alerts);
}

std::shared_ptr<AlertPresentationModel> AlertsPanel::GetAlert() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = reinterpret_cast<int>(item.GetID()) - 1;

		return _alerts[index];
	}

	return nullptr;
}

void AlertsPanel::OnListItemDoubleClick(wxDataViewEvent& event) {
	auto alert = GetAlert();

	if (alert) {
		_context.GetCommandsInvoker().EditTransaction(alert->id);
	}
	else {
		_context.GetCommandsInvoker().NewTransaction(-1);
	}
}

void AlertsPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu* menu = new AlertContextMenu(_context.GetCommandsInvoker(), GetAlert());

	_list->PopupMenu(menu);

	delete menu;
}