#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "../DataPanel.h"
#include "AlertsListDataModel.h"
#include "AlertContextMenu.h"

using namespace Clerk::Data;

class AlertsPanel : public DataPanel
{
public:	
	AlertsPanel(wxWindow *parent, DataContext& context);
	~AlertsPanel();

	void Update();

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<AlertsListDataModel> _model;
	std::vector<std::shared_ptr<AlertPresentationModel>> _alerts;
	AlertsService* _alertsService;
	unsigned int _subscriptionId;

	std::shared_ptr<AlertPresentationModel> GetAlert();	
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
};