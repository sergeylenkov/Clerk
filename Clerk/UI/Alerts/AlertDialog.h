#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../../Data/DataHelper.h"

class AlertDialog : public wxFrame
{
public:
	AlertDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

	void SetAlert(std::shared_ptr<Alert> alert);
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
	std::shared_ptr<Alert> alert;	
	vector<shared_ptr<Account>> accounts;

	void UpdateAccounts();
	void OnPeriodSelect(wxCommandEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

