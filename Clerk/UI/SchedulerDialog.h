#pragma once

#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/statline.h>
#include <wx/valnum.h>
#include <wx/wrapsizer.h>
#include "../Data/DataHelper.h"
#include "../TagsPopup.h"

class SchedulerDialog : public wxFrame
{
public:
	SchedulerDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~SchedulerDialog();

	void SetScheduler(std::shared_ptr<Scheduler> scheduler);

	std::function<void()> OnClose;

private:	
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
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
	wxStaticText *fromLabel;
	wxStaticText *toLabel;
	wxBitmapComboBox *fromList;
	wxBitmapComboBox *toList;
	wxButton *okButton;
	wxButton *cancelButton;
	wxStaticText *tagsLabel;
	wxTextCtrl *tagsField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxStaticText *dateLabel;
	wxStaticText *fromAmountLabel;
	wxTextCtrl *fromAmountField;
	wxStaticText *toAmountLabel;
	wxTextCtrl *toAmountField;
	wxRadioButton *mondayCheckBox;
	wxRadioButton *tuesdayCheckBox;
	wxRadioButton *wednesdayCheckBox;
	wxRadioButton *thursdayCheckBox;
	wxRadioButton *fridayCheckBox;
	wxRadioButton *saturdayCheckBox;
	wxRadioButton *sundayCheckBox;
	
	float fromValue = 0.0;
	float toValue = 0.0;
	SchedulerTypes type;
	std::vector<std::shared_ptr<Account>> accounts;
	std::vector<std::shared_ptr<Account>> fromAccounts;
	std::vector<std::shared_ptr<Account>> toAccounts;
	std::shared_ptr<Scheduler> scheduler;
	std::shared_ptr<Account> fromAccount;
	std::shared_ptr<Account> toAccount;
	wxImageList *accountsImageList;
	TagsPopup *tagsPopup;

	void SelectFromAccount(int index);
	void SelectToAccount(int index);
	void SelectToAccount(std::shared_ptr<Account> account);
	void UpdateFromList();
	void UpdateToList(std::shared_ptr<Account> account);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnFromAccountSelect(wxCommandEvent &event);
	void OnToAccountSelect(wxCommandEvent &event);
	void OnFromAmountKillFocus(wxFocusEvent &event);
	void OnToAmountKillFocus(wxFocusEvent &event);
	void OnTextChanged(wxKeyEvent &event);
	void OnTagsKillFocus(wxFocusEvent& event);
	void OnSelectTag();
	void AddTag();
	wxString ClearAmountValue(wxString &value);	
	void OnPatternSelect(wxCommandEvent& event);
	void SelectPatternType(SchedulerTypes type);
	void SelectWeekday(int day);
};
