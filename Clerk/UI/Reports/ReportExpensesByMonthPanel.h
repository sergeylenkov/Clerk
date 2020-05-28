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
#include "../../Data/DataHelper.h"
#include "../Charts/LineChart.h"
#include "ExpensesTooltipPopup.h"
#include "../../Data/Settings.h"
#include "../Controls/AccountsComboBox.h"

using namespace std;

class ReportExpensesByMonthPanel : public DataPanel
{
public:
	ReportExpensesByMonthPanel(wxWindow *parent, wxWindowID id);
	~ReportExpensesByMonthPanel();

	void Update();

private:
	LineChart *chart;
	AccountsComboBox *accountsComboBox;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	wxCheckBox *averageCheckbox;
	vector<shared_ptr<Account>> accounts;
	ExpensesTooltipPopup *chartPopup;
	vector<DateValue> values = {};
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