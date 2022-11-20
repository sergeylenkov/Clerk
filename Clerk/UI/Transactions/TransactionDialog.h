#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/defs.h>
#include <functional>
#include "../../Data/Settings.h"
#include "../Controls/TagsPopup.h"
#include "../Controls/AmountField.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../ViewModels/TransactionEditViewModel.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~TransactionDialog();
		
	void SetViewModel(TransactionEditViewModel* viewModel);

private:
	TransactionEditViewModel* _viewModel;
	Icons& _icons;
	wxPanel *_mainPanel;
	wxStaticText *_fromLabel;
	wxStaticText *_toLabel;
	wxBitmapComboBox *_fromList;
	wxBitmapComboBox *_toList;
	wxButton *_okButton;
	wxButton *_cancelButton;
	wxStaticText *_tagsLabel;
	wxTextCtrl *_tagsField;
	wxStaticText *_noteLabel;
	wxTextCtrl *_noteField;
	wxStaticText *_dateLabel;
	wxDatePickerCtrl *_datePicker;
	wxStaticText *_fromAmountLabel;
	AmountField *_fromAmountField;
	wxStaticText *_toAmountLabel;
	AmountField *_toAmountField;
	std::shared_ptr<TransactionPresentationModel> _transaction;
	std::shared_ptr<TransactionPresentationModel> _splitTransaction;
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
