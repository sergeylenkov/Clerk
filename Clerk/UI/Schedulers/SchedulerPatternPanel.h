#pragma once

#include "wx/wx.h"
#include <wx/statline.h>
#include <wx/valnum.h>
#include <wx/wrapsizer.h>
#include "SchedulerViewModel.h"

using namespace Clerk::Data;

class SchedulerPatternPanel : public wxPanel
{
public:
	SchedulerPatternPanel(wxWindow* parent, const wxPoint& position, const wxSize& size);

	void SetViewModel(SchedulerViewModel* viewModel);

private:
	SchedulerViewModel* _viewModel;

	wxRadioButton* _dailyButton;
	wxRadioButton* _weeklyButton;
	wxRadioButton* _monthlyButton;
	wxRadioButton* _yearlyButton;
	wxPanel* _dailyPatternPanel;
	wxPanel* _weeklyPatternPanel;
	wxPanel* _monthlyPatternPanel;
	wxPanel* _yearlyPatternPanel;
	wxTextCtrl* _dailyDayField;
	wxTextCtrl* _weeklyWeekField;
	wxTextCtrl* _monthlyDayField;
	wxTextCtrl* _monthlyMonthField;
	wxComboBox* _yearlyMonthChoice;
	wxTextCtrl* _yearlyDayField;
	wxRadioButton* _mondayCheckBox;
	wxRadioButton* _tuesdayCheckBox;
	wxRadioButton* _wednesdayCheckBox;
	wxRadioButton* _thursdayCheckBox;
	wxRadioButton* _fridayCheckBox;
	wxRadioButton* _saturdayCheckBox;
	wxRadioButton* _sundayCheckBox;

	void Update();
	void SelectWeekday(int day);
	void UpdatePatternType(SchedulerType type);
	void OnPatternSelect(wxCommandEvent& event);
};
