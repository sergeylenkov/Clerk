#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../DataPanel.h"
#include "../../Data/DataHelper.h"
#include "../Charts/LineChart.h"
#include "ExpensesTooltipPopup.h"
#include "../../Data/Settings.h"

using namespace std;

class ReportExpensesByMonthPanel : public DataPanel
{
public:
	ReportExpensesByMonthPanel(wxWindow *parent, wxWindowID id);
	~ReportExpensesByMonthPanel();

	void Update();

private:
	LineChart *chart;
	wxBitmapComboBox *accountList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	vector<shared_ptr<Account>> accounts;
	ExpensesTooltipPopup *chartPopup;
	vector<DateValue> values = {};
	wxDateTime periodFromDate;
	wxDateTime periodToDate;

	void OnAccountSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void CalculatePeriod();
};