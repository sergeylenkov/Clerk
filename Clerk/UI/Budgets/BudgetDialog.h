#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include "../PresentationModels/BudgetPresentationModel.h"
#include "../../Data/Models/AccountModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class BudgetDialog : public wxFrame
{
public:
	BudgetDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);	

	void SetBudget(std::shared_ptr<BudgetPresentationModel> budget);
	std::function<void()> OnClose;

private:
	wxPanel *mainPanel;
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;	
	wxStaticText *periodLabel;
	wxComboBox *periodList;
	wxDatePickerCtrl *datePicker;
	wxListCtrl *accountsList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<BudgetPresentationModel> _budget;
	std::vector<std::shared_ptr<AccountModel>> accounts;

	void UpdateAccounts();
	void OnPeriodSelect(wxCommandEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

