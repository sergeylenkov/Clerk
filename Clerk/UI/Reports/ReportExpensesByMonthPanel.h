#pragma once

#include <wx/wx.h>
#include <algorithm>
#include <map>
#include <set>
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

class ReportExpensesByMonthPanel : public DataPanel
{
public:
	ReportExpensesByMonthPanel(wxWindow *paren, DataContext& context, Icons& icons);
	~ReportExpensesByMonthPanel();

	void Update();

private:
	LineChart *_chart;
	AccountsComboBox *_accountsComboBox;
	PeriodFilterPanel*_periodFilterPanel;
	wxCheckBox *_averageCheckbox;
	ReportChartTooltipPopup* _chartPopup;
	shared_vector<AccountPresentationModel> _accounts;	
	std::vector<DateValueViewModel> _values = {};
	std::set<int> _selectedIds;

	void OnDrawAverageCheck(wxCommandEvent& event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
};