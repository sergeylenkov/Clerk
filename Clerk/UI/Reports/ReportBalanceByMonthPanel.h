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
#include "ExpensesTooltipPopup.h"

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
	shared_vector<AccountPresentationModel> _accounts;
	ExpensesTooltipPopup *_chartPopup;
	std::vector<DateValueViewModel> _values;
	std::set<int> _selectedIds;

	void OnAccountSelect(std::set<int> ids);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	wxString GetSelectedAccounsIds();
};