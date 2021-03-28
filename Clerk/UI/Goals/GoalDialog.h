#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include "../../Data/ViewModels/GoalViewModel.h"
#include "../../Data/ViewModels/AccountViewModel.h"

using namespace Clerk::Data;

class GoalDialog : public wxFrame
{
public:
	GoalDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

	void SetGoal(std::shared_ptr<GoalViewModel> goal);
	std::function<void()> OnClose;

private:
	wxPanel *mainPanel;
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
	std::shared_ptr<GoalViewModel> goal;
	std::vector<std::shared_ptr<AccountViewModel>> accounts;

	void UpdateAccounts();
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

