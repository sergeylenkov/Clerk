#pragma once

#include <wx/wx.h>
#include <algorithm>
#include <map>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "../Charts/GroupedBarChart.h"
#include "../DataPanel/DataPanel.h"
#include "../Controls/AccountsComboBox.h"
#include "../Controls/PeriodFilterPanel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ReportReceiptsAndExpensesByMonthPanel : public DataPanel
{
public:
	ReportReceiptsAndExpensesByMonthPanel(wxWindow* parent, DataContext& context, Icons& icons);
	~ReportReceiptsAndExpensesByMonthPanel();

	void Update();

private:
	GroupedBarChart* _chart;
	AccountsComboBox* _accountsComboBox;
	PeriodFilterPanel* _periodFilterPanel;	
	shared_vector<AccountPresentationModel> _accounts;
	std::vector<DateValueViewModel> _receipts = {};
	std::vector<DateValueViewModel> _expenses = {};
	std::set<int> _selectedIds;

	void RestoreFilterSettings();
	void SaveFilterSettings();
};

