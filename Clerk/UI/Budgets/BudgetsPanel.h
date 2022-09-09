#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../DataPanel.h"
#include "BudgetsListDataModel.h"
#include "BudgetsProgressRender.h"
#include "../../Data/ViewModels/BudgetViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

class BudgetsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	BudgetsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker);

	std::shared_ptr<BudgetViewModel> GetBudget();
	void Update();	

	std::function<void(std::shared_ptr<BudgetViewModel> budget)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<BudgetsListDataModel> model;
	std::vector<std::shared_ptr<BudgetViewModel>> _budgets;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};