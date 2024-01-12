#pragma once

#include "wx/wx.h"
#include <wx/bmpcbox.h>
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"
#include "../Controls/AmountField.h"
#include "../TagsField/TagsField.h"
#include "SchedulerViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class SchedulerTransactionPanel : public wxPanel
{
public:
	SchedulerTransactionPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, Icons& icons, DataContext& context);

	void SetViewModel(SchedulerViewModel* viewModel);

private:
	SchedulerViewModel* _viewModel;
	Icons& _icons;
	DataContext& _context;
	wxBitmapComboBox* _fromList;
	wxBitmapComboBox* _toList;
	TagsField* _tagsField;
	wxStaticText* _fromCurrencyLabel;
	AmountField* _fromAmountField;
	wxStaticText* _toCurrencyLabel;
	AmountField* _toAmountField;

	void Update();
	void UpdateToList();
	void UpdateFromList();
	void SelectFromAccount(int index);
	void SelectToAccount(int index);
	void OnFromAccountSelect(wxCommandEvent& event);
	void OnToAccountSelect(wxCommandEvent& event);
	void OnFromAmountKillFocus(wxFocusEvent& event);
	void OnToAmountKillFocus(wxFocusEvent& event);
};
