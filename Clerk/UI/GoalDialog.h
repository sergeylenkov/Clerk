#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../Data/DataHelper.h"

class GoalDialog : public wxFrame
{
public:
	GoalDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~GoalDialog();

	void SetGoal(std::shared_ptr<Goal> goal);

	std::function<void()> OnClose;

private:
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
	wxStaticText *periodLabel;	
	wxDatePickerCtrl *datePicker;
	wxListCtrl *accountsList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<Goal> goal;
	vector<shared_ptr<Account>> accounts;

	void UpdateAccounts();
	void OnPeriodSelect(wxCommandEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};

