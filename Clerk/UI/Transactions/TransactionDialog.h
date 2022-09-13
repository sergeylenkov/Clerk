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

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~TransactionDialog();
		
	void SetTransaction(std::shared_ptr<TransactionViewModel> transaction);

private:
	TransactionEditViewModel* _viewModel;
	Icons& _icons;
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
	std::shared_ptr<TransactionViewModel> _transaction;
	std::shared_ptr<TransactionViewModel> _splitTransaction;
	TagsPopup *_tagsPopup;

	void Update();
	void UpdateToList();
	void UpdateFromList();
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
