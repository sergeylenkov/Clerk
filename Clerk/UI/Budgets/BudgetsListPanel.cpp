#include "BudgetsListPanel.h"

BudgetsListPanel::BudgetsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataListPanel(parent, context, icons)
{
	_model = new BudgetsListDataModel();
	_list->AssociateModel(_model.get());

	_budgetsService = &_context.GetBudgetsService();
	_subscriptionId = _budgetsService->Subscribe([&]() {
		Update();
	});

	CreateListColumns();
	Update();
}

BudgetsListPanel::~BudgetsListPanel() {
	_budgetsService->Unsubscribe(_subscriptionId);
	SaveColumnsSettings();
}

void BudgetsListPanel::Update() {
	_budgets = _budgetsService->GetAll();
	_model.get()->SetItems(_budgets);
}

std::shared_ptr<BudgetPresentationModel> BudgetsListPanel::GetBudget() {	
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;
		return _budgets[index];
	}	

	return nullptr;
}

void BudgetsListPanel::CreateListColumns() {
	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetBudgetsListColumns();

	for (auto& column : columns) {
		wxDataViewColumn* dataViewColumn = nullptr;

		switch (static_cast<BudgetsListColumns>(column.index))
		{
			case BudgetsListColumns::Name:
				dataViewColumn = _list->AppendTextColumn(_("Name"), static_cast<int>(BudgetsListColumns::Name), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case BudgetsListColumns::Period:
				dataViewColumn = _list->AppendTextColumn(_("Period"), static_cast<int>(BudgetsListColumns::Period), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case BudgetsListColumns::Progress: {
				DataViewProgressRender* render = new DataViewProgressRender(true);

				dataViewColumn = new wxDataViewColumn(_("Progress"), render, static_cast<int>(BudgetsListColumns::Progress), column.width, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);
				}
				break;
			case BudgetsListColumns::Limit:
				dataViewColumn = _list->AppendTextColumn(_("Limit"), static_cast<int>(BudgetsListColumns::Limit), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case BudgetsListColumns::Current:
				dataViewColumn = _list->AppendTextColumn(_("Current"), static_cast<int>(BudgetsListColumns::Current), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case BudgetsListColumns::Remain:
				dataViewColumn = _list->AppendTextColumn(_("Remain"), static_cast<int>(BudgetsListColumns::Remain), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
		}
	}

	_list->AppendTextColumn("", static_cast<int>(BudgetsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	RestoreSorting(columns);
}

void BudgetsListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetBudgetsListColumns();
	columns = UpdateColumnsSettings(columns);

	Settings::GetInstance().SetBudgetsListColumns(columns);
}

void BudgetsListPanel::EditSelectedItem() {
	auto budget = GetBudget();

	if (budget) {
		_context.GetCommandsInvoker().EditBudget(budget->id);
	}
	else {
		_context.GetCommandsInvoker().NewBudget();
	}
}

void BudgetsListPanel::ShowContextMenu() {
	wxMenu* menu = new BudgetContextMenu(_context.GetCommandsInvoker(), _icons, GetBudget());

	_list->PopupMenu(menu);

	delete menu;
}