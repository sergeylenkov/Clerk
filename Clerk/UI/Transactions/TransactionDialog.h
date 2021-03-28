#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/defs.h>
#include <functional>
#include "../../Data/Settings.h"
#include "../TagsPopup.h"
#include "../Controls/AmountField.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/TransactionViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~TransactionDialog();
		
	void SetTransaction(std::shared_ptr<TransactionViewModel> transaction);
	void SetSplitTransaction(std::shared_ptr<TransactionViewModel> transaction);

	function<void()> OnClose;

private:
	wxPanel *mainPanel;
	wxStaticText *fromLabel;
	wxStaticText *toLabel;
	wxBitmapComboBox *fromList;
	wxBitmapComboBox *toList;
	wxButton *okButton;
	wxButton *cancelButton;
	wxStaticText *tagsLabel;
	wxTextCtrl *tagsField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxStaticText *dateLabel;
	wxDatePickerCtrl *datePicker;
	wxStaticText *fromAmountLabel;
	AmountField *fromAmountField;
	wxStaticText *toAmountLabel;
	AmountField *toAmountField;
	float fromValue = 0.0;
	float toValue = 0.0;
	std::vector<std::shared_ptr<AccountViewModel>> _accounts;
	std::vector<std::shared_ptr<AccountViewModel>> _fromAccounts;
	std::vector<std::shared_ptr<AccountViewModel>> _toAccounts;
	std::shared_ptr<TransactionViewModel> _transaction;
	std::shared_ptr<TransactionViewModel> _splitTransaction;
	std::shared_ptr<AccountViewModel> _fromAccount;
	std::shared_ptr<AccountViewModel> _toAccount;
	TagsPopup *tagsPopup;

	void SelectFromAccount(int index);
	void SelectToAccount(int index);
	void SelectToAccount(AccountViewModel& account);
	void UpdateFromList();
	void UpdateToList(AccountViewModel& account);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnFromAccountSelect(wxCommandEvent &event);
	void OnToAccountSelect(wxCommandEvent &event);
	void OnFromAmountKillFocus(wxFocusEvent &event);
	void OnToAmountKillFocus(wxFocusEvent &event);
	void OnTextChanged(wxKeyEvent &event);
	void OnTagsKeyDown(wxKeyEvent &event);
	void OnTagsKillFocus(wxFocusEvent& event);	
	void OnSelectTag();
	void AddTag();	
	void OnKeyDown(wxKeyEvent &event);
};
