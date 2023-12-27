#include "DataListPanel.h"

DataListPanel::DataListPanel(wxWindow* parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxBORDER_NONE);

	_mainSizer = new wxBoxSizer(wxVERTICAL);

	_mainSizer->Add(_list, 1, wxEXPAND);

	SetSizer(_mainSizer);
	Layout();

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &DataListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &DataListPanel::OnRightClick, this);
}

void DataListPanel::RestoreSorting(std::vector<ListColumnsSettings> columns) {
	for (unsigned int i = 0; i < columns.size(); i++) {
		ListColumnsSettings column = columns[i];
		wxDataViewColumn* dataViewColumn = _list->GetColumn(i);

		if (column.sorted) {
			dataViewColumn->SetSortOrder(!column.sortedDesc);
		}
	}
}

std::vector<ListColumnsSettings> DataListPanel::UpdateColumnsSettings(std::vector<ListColumnsSettings> columns) {
	wxDataViewColumn* sortingColumn = _list->GetSortingColumn();

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);		

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = column == sortingColumn;
		columns[i].sortedDesc = column->IsSortOrderAscending();
	}

	return columns;
}

void DataListPanel::OnListItemDoubleClick(wxDataViewEvent& event) {
	EditSelectedItem();
}

void DataListPanel::OnRightClick(wxDataViewEvent& event) {
	ShowContextMenu();
}