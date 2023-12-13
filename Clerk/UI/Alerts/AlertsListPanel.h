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
	AlertsListPanel(wxWindow *parent, DataContext& context);
	~AlertsListPanel();

	void Update();

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<AlertsListDataModel> _model;
	std::vector<std::shared_ptr<AlertPresentationModel>> _alerts;
	AlertsService* _alertsService;
	unsigned int _subscriptionId;
	int _sortBy;
	bool _sortDesc;

	std::shared_ptr<AlertPresentationModel> GetAlert();	
	void CreateListColumns();
	void SaveColumnsSettings();	
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
};