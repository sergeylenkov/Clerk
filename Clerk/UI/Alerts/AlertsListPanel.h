#pragma once

#include <wx/wx.h>
#include "../DataListPanel.h"
#include "AlertsListDataModel.h"
#include "AlertContextMenu.h"

using namespace Clerk::Data;

class AlertsListPanel : public DataListPanel
{
public:	
	AlertsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~AlertsListPanel();

	void Update();

private:
	wxObjectDataPtr<AlertsListDataModel> _model;
	shared_vector<AlertPresentationModel> _alerts;
	AlertsService* _alertsService;
	unsigned int _subscriptionId;

	std::shared_ptr<AlertPresentationModel> GetAlert();	
	void CreateListColumns() override;
	void SaveColumnsSettings() override;
	void ShowContextMenu() override;
	void EditSelectedItem() override;
};