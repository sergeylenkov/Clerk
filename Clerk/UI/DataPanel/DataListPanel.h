#pragma once

#include "wx/dataview.h"
#include "DataPanel.h"
#include "../../Utils/Settings/Settings.h"

class DataListPanel : public DataPanel
{
public:
	DataListPanel(wxWindow* parent, DataContext& context, Icons& icons);

protected:
	wxBoxSizer* _mainSizer;
	wxDataViewCtrl* _list;

	virtual void CreateListColumns() = 0;
	virtual void SaveColumnsSettings() = 0;
	virtual void ShowContextMenu() = 0;
	virtual void EditSelectedItem() = 0;
	void RestoreSorting(std::vector<ListColumnsSettings> columns);
	std::vector<ListColumnsSettings> UpdateColumnsSettings(std::vector<ListColumnsSettings> columns);

private:	
	void OnListItemDoubleClick(wxDataViewEvent& event);
	void OnRightClick(wxDataViewEvent& event);
};