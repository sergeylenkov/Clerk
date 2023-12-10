#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <wx/combo.h>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include "../DataPanel.h"
#include "../Charts/LineChart.h"
#include "ExpensesTooltipPopup.h"
#include "../../Data/Settings.h"
#include "../Controls/AccountsComboBox.h"
#include "../PresentationModels/Types.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class ReportExpensesByMonthPanel : public DataPanel
{
public:
	ReportExpensesByMonthPanel(wxWindow *paren, DataContext& context);
	~ReportExpensesByMonthPanel();

	void Update();

private:
	LineChart *_chart;
	AccountsComboBox *_accountsComboBox;
	wxComboBox *_periodList;
	wxDatePickerCtrl *_fromDatePicker;
	wxDatePickerCtrl *_toDatePicker;
	wxCheckBox *_averageCheckbox;
	shared_vector<AccountPresentationModel> _accounts;
	ExpensesTooltipPopup *_chartPopup;
	std::vector<DateValueViewModel> _values = {};
	wxDateTime _periodFromDate;
	wxDateTime _periodToDate;
	std::set<int> _selectedIds;

	void OnAccountSelect(std::set<int> ids);
	void OnDateChanged(wxDateEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDrawAverageCheck(wxCommandEvent& event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void CalculatePeriod();
	wxString GetSelectedAccounsIds();
};