#pragma once

#include <wx/wx.h>
#include <wx/valnum.h>
#include <wx/bmpcbox.h>
#include "../../Utils/Icons.h"
#include "../../Utils/Utils.h"
#include "../Controls/AmountField.h"
#include "InterestAccrualViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class InterestAccrualDialog : public wxFrame
{
public:
	InterestAccrualDialog(wxFrame* parent, const wxChar* title, int xpos, int ypos, int width, int height);
	~InterestAccrualDialog();

	void SetViewModel(InterestAccrualViewModel* viewModel);

private:
	InterestAccrualViewModel* _viewModel;
	wxStaticText* _balanceLabel;
	wxComboBox* _typeList;
	AmountField* _amountField;
	AmountField* _percentField;
	wxStaticText* _currencyLabel;

	void Update();
	void UpdateType();
	void UpdateBalance();
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnAmountKillFocus(wxFocusEvent& event);
	void OnPercentKillFocus(wxFocusEvent& event);
	void OnTypeSelect(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
};