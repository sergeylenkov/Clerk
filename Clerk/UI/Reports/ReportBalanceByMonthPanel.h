#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "../DataPanel.h"
#include "../Charts/LineChart.h"
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
	LineChart *chart;
	wxBitmapComboBox *accountList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	shared_vector<AccountPresentationModel> _accounts;
	ExpensesTooltipPopup *chartPopup;
	std::vector<DateValueViewModel> values;
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