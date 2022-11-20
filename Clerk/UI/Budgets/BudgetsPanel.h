#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../DataPanel.h"
#include "BudgetsListDataModel.h"
#include "BudgetsProgressRender.h"
#include "../PresentationModels/BudgetPresentationModel.h"

using namespace Clerk::Data;

class BudgetsPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
	};

	BudgetsPanel(wxWindow *parent, DataContext& context);

	std::shared_ptr<BudgetPresentationModel> GetBudget();
	void Update();	

	std::function<void(std::shared_ptr<BudgetPresentationModel> budget)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<BudgetsListDataModel> model;
	std::vector<std::shared_ptr<BudgetPresentationModel>> _budgets;

	void Add();
	void Edit();
	void Delete();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};