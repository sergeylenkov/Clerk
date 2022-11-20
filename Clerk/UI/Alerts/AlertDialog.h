#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../PresentationModels/AlertPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class AlertDialog : public wxFrame
{
public:
	AlertDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

	void SetAlert(std::shared_ptr<AlertPresentationModel> alert);
	std::function<void()> OnClose;

private:
	wxPanel *mainPanel;
	wxTextCtrl *nameField;	
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *periodLabel;
	wxComboBox *periodList;
	wxStaticText *conditionLabel;
	wxComboBox *conditionList;	
	wxListCtrl *accountsList;
	wxTextCtrl *amountField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<AlertPresentationModel> alert;
	std::vector<std::shared_ptr<AccountPresentationModel>> accounts;

	void UpdateAccounts();
	void OnTypeSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

