#pragma once

#include <wx/wx.h>
#include <algorithm>
#include <map>
#include <set>
#include "../DataPanel/DataPanel.h"
#include "../Charts/LineChart.h"
#include "../Controls/AccountsComboBox.h"
#include "../Controls/PeriodFilterPanel.h"
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "Tooltip/ReportChartTooltipPopup.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class ReportReceiptsByMonthPanel : public DataPanel
{
public:
	ReportReceiptsByMonthPanel(wxWindow* paren, DataContext& context, Icons& icons);
	~ReportReceiptsByMonthPanel();

	void Update();

private:
	LineChart* _chart;
	AccountsComboBox* _accountsComboBox;
	PeriodFilterPanel* _periodFilterPanel;
	wxCheckBox* _averageCheckbox;
	shared_vector<AccountPresentationModel> _accounts;
	ReportChartTooltipPopup* _chartPopup;
	std::vector<DateValueViewModel> _values = {};	
	std::set<int> _selectedIds;

	void OnDrawAverageCheck(wxCommandEvent& event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
};
