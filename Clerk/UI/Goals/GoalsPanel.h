#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/DataHelper.h"
#include "../DataPanel.h"
#include "GoalsListDataModel.h"
#include "GoalsProgressRender.h"

class GoalsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	GoalsPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Goal> GetGoal();
	void Update();

	std::function<void(std::shared_ptr<Goal> goal)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<GoalsListDataModel> model;
	std::vector<std::shared_ptr<Goal>> goals;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};