#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "../DataPanel.h"
#include "../../Data/Settings.h"
#include "SchedulersListDataModel.h"
#include "SchedulerContextMenu.h"

using namespace Clerk::Data;

class SchedulersListPanel : public DataPanel
{
public:
	SchedulersListPanel(wxWindow *parent, DataContext& context);
	~SchedulersListPanel();

	void Update();

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<SchedulersListDataModel> _model;
	shared_vector<SchedulerPresentationModel> _schedulers;
	SchedulersService* _schedulersService;
	unsigned int _subscriptionId;
	int _sortBy;
	bool _sortDesc;

	std::shared_ptr<SchedulerPresentationModel> GetScheduler();
	void CreateListColumns();
	void SaveColumnsSettings();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
};