#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "../PresentationModels/GoalPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../ViewModels/GoalViewModel.h"
#include "../Controls/AmountField.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class GoalDialog : public wxFrame
{
public:
	GoalDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~GoalDialog();

	void SetViewModel(GoalViewModel* viewModel);

private:
	GoalViewModel* _viewModel;
	Icons& _icons;
	wxTextCtrl* _nameField;	
	wxDatePickerCtrl* _datePicker;
	wxListCtrl* _accountsList;
	AmountField* _amountField;
	shared_vector<AccountPresentationModel> _accounts;

	void Update();
	void UpdateAccounts();
	void OnAccountsChange(wxListEvent& event);
	void OnNameKillFocus(wxFocusEvent& event);
	void OnAmountKillFocus(wxFocusEvent& event);
	void OnDateChanged(wxDateEvent& event);
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

