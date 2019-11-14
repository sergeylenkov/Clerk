#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/defs.h>
#include <functional>
#include "../../Data/DataHelper.h"
#include "../../Data/Settings.h"
#include "../TagsPopup.h"

using namespace std;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~TransactionDialog();
		
	void SetTransaction(std::shared_ptr<Transaction> transaction);
	void SetSplitTransaction(std::shared_ptr<Transaction> transaction);

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
	wxTextCtrl *fromAmountField;
	wxStaticText *toAmountLabel;
	wxTextCtrl *toAmountField;
	float fromValue = 0.0;
	float toValue = 0.0;
	std::vector<std::shared_ptr<Account>> accounts;
	std::vector<std::shared_ptr<Account>> fromAccounts;
	std::vector<std::shared_ptr<Account>> toAccounts;
	std::shared_ptr<Transaction> transaction;
	std::shared_ptr<Transaction> splitTransaction;	
	std::shared_ptr<Account> fromAccount;
	std::shared_ptr<Account> toAccount;
	TagsPopup *tagsPopup;

	void SelectFromAccount(int index);
	void SelectToAccount(int index);
	void SelectToAccount(Account *account);
	void UpdateFromList();
	void UpdateToList(Account *account);
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
	wxString ClearAmountValue(wxString &value);
	float GetValueFromString(wxString &value);
	void OnKeyDown(wxKeyEvent &event);
};
