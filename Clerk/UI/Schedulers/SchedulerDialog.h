#pragma once

#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/statline.h>
#include <wx/valnum.h>
#include <wx/wrapsizer.h>
#include "../../Utils/Icons.h"
#include "../../Data/DataContext.h"
#include "../Controls/AmountField.h"
#include "../TagsField/TagsField.h"
#include "SchedulerViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class SchedulerDialog : public wxFrame
{
public:
	SchedulerDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons, DataContext& context);
	~SchedulerDialog();

	void SetViewModel(SchedulerViewModel* viewModel);

private:
	SchedulerViewModel* _viewModel;
	Icons& _icons;
	DataContext& _context;
	wxTextCtrl *_nameField;
	wxRadioButton *dailyButton;
	wxRadioButton *weeklyButton;
	wxRadioButton *monthlyButton;
	wxRadioButton *yearlyButton;
	wxBoxSizer *patternSizer;
	wxPanel *patternPanel;
	wxPanel *dailyPatternPanel;
	wxPanel *weeklyPatternPanel;
	wxPanel *monthlyPatternPanel;
	wxPanel *yearlyPatternPanel;
	wxTextCtrl *dailyDayField;
	wxTextCtrl *weeklyWeekField;
	wxTextCtrl *monthlyDayField;
	wxTextCtrl *monthlyMonthField;
	wxComboBox *yearlyMonthChoice;
	wxTextCtrl *yearlyDayField;	
	wxBitmapComboBox *_fromList;
	wxBitmapComboBox *_toList;
	TagsField* _tagsField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxStaticText * _fromCurrencyLabel;
	AmountField* _fromAmountField;
	wxStaticText * _toCurrencyLabel;
	AmountField* _toAmountField;
	wxRadioButton *mondayCheckBox;
	wxRadioButton *tuesdayCheckBox;
	wxRadioButton *wednesdayCheckBox;
	wxRadioButton *thursdayCheckBox;
	wxRadioButton *fridayCheckBox;
	wxRadioButton *saturdayCheckBox;
	wxRadioButton *sundayCheckBox;
	
	SchedulerType type;

	void Update();
	void UpdateToList();
	void UpdateFromList();
	void SelectFromAccount(int index);
	void SelectToAccount(int index);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnFromAccountSelect(wxCommandEvent &event);
	void OnToAccountSelect(wxCommandEvent &event);
	void OnFromAmountKillFocus(wxFocusEvent &event);
	void OnToAmountKillFocus(wxFocusEvent &event);
	void OnPatternSelect(wxCommandEvent& event);
	void SelectPatternType(SchedulerType type);
	void SelectWeekday(int day);
	void OnKeyDown(wxKeyEvent &event);
};
