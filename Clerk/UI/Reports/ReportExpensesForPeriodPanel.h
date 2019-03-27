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
#include "../../Settings.h"

using namespace std;

class ReportExpensesForPeriodPanel : public DataPanel
{
public:
	ReportExpensesForPeriodPanel(wxWindow *parent, wxWindowID id);
	~ReportExpensesForPeriodPanel();

	void Update();

private:
	BarChart *chart;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	vector<DateValue> values = {};

	void OnDateChanged(wxDateEvent &event);
	void RestoreFilterSettings();
	void SaveFilterSettings();
};

