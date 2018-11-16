#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum class GoalsPanelMenuTypes {
	Add = 1,
	Edit = 2,
	Delete = 3,
};

class GoalsPanel : public DataPanel
{
public:
	GoalsPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Goal> GetGoal();
	void Update();

	std::function<void(std::shared_ptr<Goal> goal)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxListCtrl *list;
	std::vector<std::shared_ptr<Goal>> goals;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxListEvent &event);
	void OnRightClick(wxContextMenuEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};