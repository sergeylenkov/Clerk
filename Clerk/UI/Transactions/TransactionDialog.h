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
#include "../../Data/ViewModels/TransactionEditViewModel.h"
#include "../../Utils/Icons.h"
#include "../../Data/DataContext.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons, DataContext& context);
	~TransactionDialog();
		
	void SetViewModel(TransactionEditViewModel* viewModel);

private:
	TransactionEditViewModel* _viewModel;
	Icons& _icons;
	DataContext& _context;
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

	void Update();
	void SelectFromAccount(int index);
	void SelectToAccount(int index);	
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
