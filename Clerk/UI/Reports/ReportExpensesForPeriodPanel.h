#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../DataPanel.h"
#include "../Charts/BarChart.h"
#include "../../Data/Settings.h"
#include "../PresentationModels/Types.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ReportExpensesForPeriodPanel : public DataPanel
{
public:
	ReportExpensesForPeriodPanel(wxWindow *parent, DataContext& context);
	~ReportExpensesForPeriodPanel();

	void Update();

private:
	BarChart *chart;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	std::vector<DateValueViewModel> values = {};
	wxDateTime periodFromDate;
	wxDateTime periodToDate;

	void OnDateChanged(wxDateEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void CalculatePeriod();
};

