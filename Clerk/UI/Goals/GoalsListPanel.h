#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/Settings.h"
#include "../DataPanel.h"
#include "../PresentationModels/GoalPresentationModel.h"
#include "../Controls/DataViewProgressRender.h"
#include "GoalsListDataModel.h"
#include "GoalContextMenu.h"
#include "Enums.h"

using namespace Clerk::Data;

class GoalsListPanel : public DataPanel
{
public:
	GoalsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~GoalsListPanel();

	void Update();

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<GoalsListDataModel> _model;
	shared_vector<GoalPresentationModel> _goals;
	GoalsService* _goalsService;
	unsigned int _subscriptionId;
	GoalsListColumns _sortBy;
	bool _sortDesc;

	std::shared_ptr<GoalPresentationModel> GetGoal();
	void CreateListColumns();
	void SaveColumnsSettings();
	void OnListItemDoubleClick(wxDataViewEvent& event);
	void OnRightClick(wxDataViewEvent& event);
};