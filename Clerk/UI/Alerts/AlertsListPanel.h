#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "../DataPanel.h"
#include "../../Data/Settings.h"
#include "AlertsListDataModel.h"
#include "AlertContextMenu.h"

using namespace Clerk::Data;

class AlertsListPanel : public DataPanel
{
public:	
	AlertsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~AlertsListPanel();

	void Update();

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<AlertsListDataModel> _model;
	shared_vector<AlertPresentationModel> _alerts;
	AlertsService* _alertsService;
	unsigned int _subscriptionId;
	AlertsListColumns _sortBy;
	bool _sortDesc;

	std::shared_ptr<AlertPresentationModel> GetAlert();	
	void CreateListColumns();
	void SaveColumnsSettings();	
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
};