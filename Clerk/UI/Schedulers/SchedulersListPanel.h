#pragma once

#include <wx/wx.h>
#include "../../Data/PresentationModels/SchedulerPresentationModel.h"
#include "../DataListPanel.h"
#include "SchedulersListDataModel.h"
#include "SchedulerContextMenu.h"

using namespace Clerk::Data;

class SchedulersListPanel : public DataListPanel
{
public:
	SchedulersListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~SchedulersListPanel();

	void Update();

private:	
	wxObjectDataPtr<SchedulersListDataModel> _model;
	shared_vector<SchedulerPresentationModel> _schedulers;
	SchedulersService* _schedulersService;
	unsigned int _subscriptionId;

	std::shared_ptr<SchedulerPresentationModel> GetScheduler();
	void CreateListColumns() override;
	void SaveColumnsSettings() override;
	void ShowContextMenu() override;
	void EditSelectedItem() override;
};