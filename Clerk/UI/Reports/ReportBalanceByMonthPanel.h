#pragma once

#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "../DataPanel/DataPanel.h"
#include "../Charts/LineChart.h"
#include "../Controls/AccountsComboBox.h"
#include "../Controls/PeriodFilterPanel.h"
#include "Tooltip/ReportChartTooltipPopup.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class ReportBalanceByMonthPanel : public DataPanel
{
public:
	ReportBalanceByMonthPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~ReportBalanceByMonthPanel();

	void Update();

private:
	LineChart *_chart;
	AccountsComboBox* _accountsComboBox;
	PeriodFilterPanel* _periodFilterPanel;
	ReportChartTooltipPopup* _chartPopup;
	shared_vector<AccountPresentationModel> _accounts;	
	std::vector<DateValueViewModel> _values;
	std::set<int> _selectedIds;

	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
};