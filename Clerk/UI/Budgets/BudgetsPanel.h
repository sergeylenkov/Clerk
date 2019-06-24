#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/DataHelper.h"
#include "../DataPanel.h"
#include "BudgetsListDataModel.h"
#include "BudgetsProgressRender.h"

class BudgetsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	BudgetsPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Budget> GetBudget();
	void Update();	

	std::function<void(std::shared_ptr<Budget> budget)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<BudgetsListDataModel> model;
	std::vector<std::shared_ptr<Budget>> budgets;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};