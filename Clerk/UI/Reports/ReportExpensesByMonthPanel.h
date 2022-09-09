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
#include "../../Data/ViewModels/DateValueViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::Commands;

class ReportExpensesByMonthPanel : public DataPanel
{
public:
	ReportExpensesByMonthPanel(wxWindow *paren, DataContext& context, CommandsInvoker& commandsInvoker);
	~ReportExpensesByMonthPanel();

	void Update();

private:
	LineChart *chart;
	AccountsComboBox *accountsComboBox;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	wxCheckBox *averageCheckbox;
	std::vector<std::shared_ptr<AccountViewModel>> _accounts;
	ExpensesTooltipPopup *chartPopup;
	std::vector<DateValueViewModel> values = {};
	wxDateTime periodFromDate;
	wxDateTime periodToDate;
	std::set<int> selectedIds;

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