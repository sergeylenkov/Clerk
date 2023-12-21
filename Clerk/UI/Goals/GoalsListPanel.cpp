#include "GoalsListPanel.h"

GoalsListPanel::GoalsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_model = new GoalsListDataModel();
	_list->AssociateModel(_model.get());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &GoalsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &GoalsListPanel::OnRightClick, this);

	_sortBy = GoalsListColumns::Name;
	_sortDesc = false;

	_goalsService = &_context.GetGoalsService();
	_subscriptionId = _goalsService->Subscribe([&]() {
		Update();
	});

	Update();
}

GoalsListPanel::~GoalsListPanel() {
	_goalsService->Unsubscribe(_subscriptionId);

	SaveColumnsSettings();
}

void GoalsListPanel::Update() {
	CreateListColumns();

	_goals = _goalsService->GetAll();
	_model.get()->SetItems(_goals);
}

std::shared_ptr<GoalPresentationModel> GoalsListPanel::GetGoal() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;
		return _goals[index];
	}

	return nullptr;	
}

void GoalsListPanel::CreateListColumns() {
	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetGoalsListColumns();

	for (auto& column : columns) {
		wxDataViewColumn* dataViewColumn = nullptr;

		switch (static_cast<GoalsListColumns>(column.index))
		{
		case GoalsListColumns::Name:
			dataViewColumn = _list->AppendTextColumn(_("Name"), static_cast<int>(GoalsListColumns::Name), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case GoalsListColumns::DueDate:
			dataViewColumn = _list->AppendTextColumn(_("Due Date"), static_cast<int>(GoalsListColumns::DueDate), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case GoalsListColumns::DaysRemain:
			dataViewColumn = _list->AppendTextColumn(_("Days Remain"), static_cast<int>(GoalsListColumns::DaysRemain), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case GoalsListColumns::Progress: {
			DataViewProgressRender* render = new DataViewProgressRender(false);

				dataViewColumn = new wxDataViewColumn(_("Progress"), render, static_cast<int>(GoalsListColumns::Progress), column.width, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);
			}
			break;
		case GoalsListColumns::Goal:
			dataViewColumn = _list->AppendTextColumn(_("Goal"), static_cast<int>(GoalsListColumns::Goal), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case GoalsListColumns::Current:
			dataViewColumn = _list->AppendTextColumn(_("Current"), static_cast<int>(GoalsListColumns::Current), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		case GoalsListColumns::Remain:
			dataViewColumn = _list->AppendTextColumn(_("Remain"), static_cast<int>(GoalsListColumns::Remain), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
			break;
		}

		if (column.sorted) {
			_sortBy = static_cast<GoalsListColumns>(column.index);
			_sortDesc = column.sortedDesc;

			if (dataViewColumn != nullptr) {
				dataViewColumn->SetSortOrder(!_sortDesc);
			}
		}
	}

	_list->AppendTextColumn("", static_cast<int>(GoalsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void GoalsListPanel::SaveColumnsSettings() {
	auto columns = Settings::GetInstance().GetGoalsListColumns();

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = columns[i].index == static_cast<int>(_sortBy);;
		columns[i].sortedDesc = _sortDesc;
	}

	Settings::GetInstance().SetGoalsListColumns(columns);
}

void GoalsListPanel::OnListItemDoubleClick(wxDataViewEvent& event) {
	auto goal = GetGoal();

	if (goal) {
		_context.GetCommandsInvoker().EditGoal(goal->id);
	}
	else {
		_context.GetCommandsInvoker().NewGoal();
	}
}

void GoalsListPanel::OnRightClick(wxDataViewEvent& event) {
	wxMenu* menu = new GoalContextMenu(_context.GetCommandsInvoker(), _icons, GetGoal());

	_list->PopupMenu(menu);

	delete menu;
}
