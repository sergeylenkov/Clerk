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

using namespace std;

class ReportComparePanel : public DataPanel
{
public:
	ReportComparePanel(wxWindow *parent, wxWindowID id);
	~ReportComparePanel();

	void Update();

private:
	BarChart *chart;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	vector<DateValue> values = {};

	void OnDateChanged(wxDateEvent &event);
};

