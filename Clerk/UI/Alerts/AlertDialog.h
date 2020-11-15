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
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *periodLabel;
	wxComboBox *periodList;
	wxStaticText *conditionLabel;
	wxComboBox *conditionList;	
	wxListCtrl *accountsList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<Alert> alert;	
	vector<shared_ptr<Account>> accounts;

	void UpdateAccounts();
	void OnTypeSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
