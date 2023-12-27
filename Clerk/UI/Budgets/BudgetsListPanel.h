#pragma once

#include <wx/wx.h>
#include "../DataPanel/DataListPanel.h"
#include "../Controls/DataViewProgressRender.h"
#include "BudgetsListDataModel.h"
#include "BudgetContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class BudgetsListPanel : public DataListPanel
{
public:
	BudgetsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~BudgetsListPanel();

	void Update();	

private:
	wxObjectDataPtr<BudgetsListDataModel> _model;
	shared_vector<BudgetPresentationModel> _budgets;
	BudgetsService* _budgetsService;
	unsigned int _subscriptionId;

	std::shared_ptr<BudgetPresentationModel> GetBudget();
	void CreateListColumns() override;
	void SaveColumnsSettings() override;
	void ShowContextMenu() override;
	void EditSelectedItem() override;
};