#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "../DataPanel.h"
#include "AlertsListDataModel.h"

using namespace Clerk::Data;

class AlertsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	AlertsPanel(wxWindow *parent, DataContext& context);

	std::shared_ptr<AlertPresentationModel> GetAlert();
	void Update();	

	std::function<void(std::shared_ptr<AlertPresentationModel> alert)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<AlertsListDataModel> model;
	std::vector<std::shared_ptr<AlertPresentationModel>> alerts;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};