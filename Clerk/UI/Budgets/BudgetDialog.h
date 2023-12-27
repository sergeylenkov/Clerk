#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "../Controls/AmountField.h"
#include "../../Utils/Icons.h"
#include "BudgetViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class BudgetDialog : public wxFrame
{
public:
	BudgetDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~BudgetDialog();

	void SetViewModel(BudgetViewModel* viewModel);

private:
	BudgetViewModel* _viewModel;	
	Icons& _icons;
	wxTextCtrl *_nameField;		
	wxComboBox *_periodList;
	wxDatePickerCtrl *_datePicker;
	wxListCtrl* _accountsList;	
	AmountField* _amountField;
	float amountValue = 0.0;	
	shared_vector<AccountPresentationModel> _accounts;

	void Update();
	void UpdateAccounts();
	void OnAccountsChange(wxListEvent& event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnNameKillFocus(wxFocusEvent& event);	
	void OnAmountKillFocus(wxFocusEvent& event);
	void OnDateChanged(wxDateEvent& event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

