#pragma once

#include <wx/wx.h>
#include <wx/valnum.h>
#include <wx/bmpcbox.h>
#include <functional>
#include "../Data/Settings.h"
#include "../Data/Models/Currency.h"
#include "../Data/ViewModels/AccountViewModel.h"
#include "../Data/ViewModels/TransactionViewModel.h"
#include "../Data/ViewModels/AccountEditViewModel.h"
#include "../Utils/Icons.h"
#include "./Controls/AmountField.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class AccountDialog : public wxFrame
{
public:
	AccountDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~AccountDialog();

	void SetViewModel(AccountEditViewModel* viewModel);

private:
	AccountEditViewModel* _viewModel;
	Icons& _icons;
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
	AmountField *amountField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxButton *okButton;
	wxButton *cancelButton;

	void Update();
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnAmountKillFocus(wxFocusEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};