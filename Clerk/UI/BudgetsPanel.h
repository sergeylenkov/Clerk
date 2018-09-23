#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum class BudgetsPanelMenuTypes {
	Add = 1,
	Edit = 2,
	Delete = 3,
};

class BudgetsPanel : public DataPanel
{
public:
	BudgetsPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Budget> GetBudget();
	void Update();	

	std::function<void(std::shared_ptr<Budget> budget)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxListCtrl *list;
	std::vector<std::shared_ptr<Budget>> budgets;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxListEvent &event);
	void OnRightClick(wxContextMenuEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};