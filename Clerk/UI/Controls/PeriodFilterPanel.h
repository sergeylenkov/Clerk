#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <functional>
#include "../../Utils/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;

class PeriodFilterPanel : public wxPanel
{
public:
	PeriodFilterPanel(wxWindow* parent);

	void SetPeriod(int period);
	int GetPeriod();
	void SetFromDate(wxDateTime date);
	wxDateTime GetFromDate();
	void SetToDate(wxDateTime date);
	wxDateTime GetToDate();

	std::function<void()> OnChange;

private:
	wxComboBox* _periodList;
	wxDatePickerCtrl* _fromDatePicker;
	wxDatePickerCtrl* _toDatePicker;
	wxDateTime _periodFromDate;
	wxDateTime _periodToDate;

	void CalculatePeriod();
	void OnPeriodSelect(wxCommandEvent& event);
	void OnDateChanged(wxDateEvent& event);
};