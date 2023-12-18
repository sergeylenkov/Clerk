#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../DataPanel.h"
#include "GoalsListDataModel.h"
#include "GoalsProgressRender.h"
#include "../PresentationModels/GoalPresentationModel.h"

using namespace Clerk::Data;

class GoalsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	GoalsPanel(wxWindow *parent, DataContext& context, Icons& icons);

	std::shared_ptr<GoalPresentationModel> GetGoal();
	void Update();

	std::function<void(std::shared_ptr<GoalPresentationModel> goal)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<GoalsListDataModel> model;
	std::vector<std::shared_ptr<GoalPresentationModel>> goals;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};