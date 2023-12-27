#pragma once

#include <wx/wx.h>
#include <wx/valnum.h>
#include <wx/bmpcbox.h>
#include "../../Utils/Icons.h"
#include "../../Utils/Utils.h"
#include "../Controls/AmountField.h"
#include "AccountViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class AccountDialog : public wxFrame
{
public:
	AccountDialog(wxFrame* parent, const wxChar* title, int xpos, int ypos, int width, int height, Icons& icons);
	~AccountDialog();

	void SetViewModel(AccountViewModel* viewModel);

private:
	AccountViewModel* _viewModel;
	Icons& _icons;
	wxTextCtrl* _nameField;
	wxComboBox* _typeList;
	wxComboBox* _currencyList;
	wxBitmapComboBox* _iconList;
	AmountField* _amountField;
	AmountField* _creditField;
	wxTextCtrl* _noteField;

	void Update();
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnAmountKillFocus(wxFocusEvent &event);
	void OnCreditKillFocus(wxFocusEvent& event);
	void OnNameKillFocus(wxFocusEvent& event);
	void OnNoteKillFocus(wxFocusEvent& event);
	void OnTypeSelect(wxCommandEvent& event);
	void OnIconSelect(wxCommandEvent& event);
	void OnCurrencySelect(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent &event);	
};