#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "../Charts/BarChart.h"
#include "../DataPanel/DataPanel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ReportExpensesForPeriodPanel : public DataPanel
{
public:
	ReportExpensesForPeriodPanel(wxWindow *parent, DataContext& context, Icons& icons);
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

