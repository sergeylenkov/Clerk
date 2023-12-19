#include "BudgetsListPanel.h"

BudgetsListPanel::BudgetsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_model = new BudgetsListDataModel();
	_list->AssociateModel(_model.get());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &BudgetsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &BudgetsListPanel::OnRightClick, this);

	_sortBy = BudgetsListColumns::Name;
	_sortDesc = false;

	_budgetsService = &_context.GetBudgetsService();
	_subscriptionId = _budgetsService->Subscribe([&]() {
		Update();
	});

	Update();
}

BudgetsListPanel::~BudgetsListPanel() {
	_budgetsService->Unsubscribe(_subscriptionId);

	SaveColumnsSettings();
}

void BudgetsListPanel::Update() {
	CreateListColumns();

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
				BudgetsProgressRender* render = new BudgetsProgressRender();

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

		if (column.sorted) {
			_sortBy = static_cast<BudgetsListColumns>(column.index);
			_sortDesc = column.sortedDesc;

			if (dataViewColumn != nullptr) {
				dataViewColumn->SetSortOrder(!_sortDesc);
			}
		}
	}

	_list->AppendTextColumn("", static_cast<int>(BudgetsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void BudgetsListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetBudgetsListColumns();

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = columns[i].index == static_cast<int>(_sortBy);;
		columns[i].sortedDesc = _sortDesc;
	}

	Settings::GetInstance().SetBudgetsListColumns(columns);
}

void BudgetsListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	auto budget = GetBudget();

	if (budget) {
		_context.GetCommandsInvoker().EditBudget(budget->id);
	}
	else {
		_context.GetCommandsInvoker().NewBudget();
	}
}

void BudgetsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu* menu = new BudgetContextMenu(_context.GetCommandsInvoker(), _icons, GetBudget());

	_list->PopupMenu(menu);

	delete menu;
}