#pragma once

#include <wx/wx.h>
#include "../DataListPanel.h"
#include "../Controls/DataViewProgressRender.h"
#include "GoalsListDataModel.h"
#include "GoalContextMenu.h"

using namespace Clerk::Data;

class GoalsListPanel : public DataListPanel
{
public:
	GoalsListPanel(wxWindow* parent, DataContext& context, Icons& icons);
	~GoalsListPanel();

	void Update();

private:
	wxObjectDataPtr<GoalsListDataModel> _model;
	shared_vector<GoalPresentationModel> _goals;
	GoalsService* _goalsService;
	unsigned int _subscriptionId;

	std::shared_ptr<GoalPresentationModel> GetGoal();
	void CreateListColumns() override;
	void SaveColumnsSettings() override;
	void ShowContextMenu() override;
	void EditSelectedItem() override;
};