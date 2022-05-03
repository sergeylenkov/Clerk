#pragma once

#include <wx/wx.h>
#include <wx/valnum.h>
#include <wx/bmpcbox.h>
#include <functional>
#include "../Data/Settings.h"
#include "../Data/Models/Currency.h"
#include "../Data/ViewModels/AccountViewModel.h"
#include "../Data/ViewModels/TransactionViewModel.h"

using namespace Clerk::Data;

class AccountDialog : public wxFrame
{
public:
	AccountDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

	void SetAccount(std::shared_ptr<AccountViewModel> account);
	std::function<void()> OnClose;

private:
	wxPanel *mainPanel;
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *currencyLabel;
	wxComboBox *currencyList;
	wxStaticText *iconLabel;
	wxBitmapComboBox *iconList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<AccountViewModel> account;
	std::vector<std::shared_ptr<Currency>> currencies;
	std::shared_ptr<TransactionViewModel> initialTransaction;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnAmountKillFocus(wxFocusEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	wxString ClearAmountValue(wxString &value);
};