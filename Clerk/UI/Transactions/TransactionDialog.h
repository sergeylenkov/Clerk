#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/defs.h>
#include <functional>
#include "../../Data/Settings.h"
#include "../../Data/DataContext.h"
#include "../Controls/AmountField.h"
#include "../TagsField/TagsField.h"
#include "../../Utils/Icons.h"
#include "TransactionViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class TransactionDialog : public wxFrame
{
public:
	TransactionDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons, DataContext& context);
	~TransactionDialog();
		
	void SetViewModel(TransactionViewModel* viewModel);

private:
	TransactionViewModel* _viewModel;
	Icons& _icons;
	DataContext& _context;
	wxBitmapComboBox *_fromList;
	wxBitmapComboBox *_toList;
	wxTextCtrl *_noteField;
	wxDatePickerCtrl *_datePicker;
	wxStaticText *_fromCurrencyLabel;
	AmountField *_fromAmountField;
	wxStaticText *_toCurrencyLabel;
	AmountField *_toAmountField;
	TagsField *_tagsField;
	std::shared_ptr<TransactionPresentationModel> _transaction;
	std::shared_ptr<TransactionPresentationModel> _splitTransaction;

	void Update();
	void UpdateToList();
	void UpdateFromList();
	void SelectFromAccount(int index);
	void SelectToAccount(int index);	
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnFromAccountSelect(wxCommandEvent &event);
	void OnToAccountSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent& event);
	void OnFromAmountKillFocus(wxFocusEvent &event);
	void OnToAmountKillFocus(wxFocusEvent &event);	
	void OnNoteKillFocus(wxFocusEvent& event);
	void OnKeyDown(wxKeyEvent &event);
};
