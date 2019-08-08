#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../DataPanel.h"
#include "../../Data/DataHelper.h"
#include "../Charts/BarChart.h"
#include "../../Data/Settings.h"

using namespace std;

class ReportExpensesForPeriodPanel : public DataPanel
{
public:
	ReportExpensesForPeriodPanel(wxWindow *parent, wxWindowID id);
	~ReportExpensesForPeriodPanel();

	void Update();

private:
	BarChart *chart;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	vector<DateValue> values = {};
	wxDateTime periodFromDate;
	wxDateTime periodToDate;

	void OnDateChanged(wxDateEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void CalculatePeriod();
};

