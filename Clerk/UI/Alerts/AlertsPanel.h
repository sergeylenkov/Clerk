#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/ViewModels/AlertViewModel.h"
#include "../DataPanel.h"
#include "../../Commands/CommandsInvoker.h"
#include "AlertsListDataModel.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

class AlertsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	AlertsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker);

	std::shared_ptr<AlertViewModel> GetAlert();
	void Update();	

	std::function<void(std::shared_ptr<AlertViewModel> alert)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<AlertsListDataModel> model;
	std::vector<std::shared_ptr<AlertViewModel>> alerts;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};