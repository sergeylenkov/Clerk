#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/DataHelper.h"
#include "../DataPanel.h"
#include "AlertsListDataModel.h"

class AlertsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	AlertsPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Alert> GetAlert();
	void Update();	

	std::function<void(std::shared_ptr<Alert> budget)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<AlertsListDataModel> model;
	std::vector<std::shared_ptr<Alert>> alerts;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};