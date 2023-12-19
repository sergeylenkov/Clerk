#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../../Data/Settings.h"
#include "../PresentationModels/BudgetPresentationModel.h"
#include "../DataPanel.h"
#include "BudgetsListDataModel.h"
#include "BudgetsProgressRender.h"
#include "BudgetContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class BudgetsListPanel : public DataPanel
{
public:
	BudgetsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~BudgetsListPanel();

	void Update();	

private:
	wxDataViewCtrl *_list;
	wxObjectDataPtr<BudgetsListDataModel> _model;
	shared_vector<BudgetPresentationModel> _budgets;
	BudgetsService* _budgetsService;
	unsigned int _subscriptionId;
	BudgetsListColumns _sortBy;
	bool _sortDesc;

	std::shared_ptr<BudgetPresentationModel> GetBudget();
	void CreateListColumns();
	void SaveColumnsSettings();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
};