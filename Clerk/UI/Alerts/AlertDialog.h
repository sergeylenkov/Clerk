#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "../Controls/AmountField.h"
#include "../../Utils/Icons.h"
#include "AlertViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class AlertDialog : public wxFrame
{
public:
	AlertDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons);
	~AlertDialog();

	void SetViewModel(AlertViewModel* viewModel);

private:
	AlertViewModel* _viewModel;
	Icons& _icons;
	wxTextCtrl* _nameField;
	wxTextCtrl* _messageField;
	wxComboBox* _typeList;
	wxComboBox* _periodList;
	wxComboBox* _conditionList;	
	wxComboBox* _importanceList;
	wxListCtrl* _accountsList;
	AmountField* _amountField;
	shared_vector<AccountPresentationModel> _accounts;

	void Update();
	void UpdateAccounts();
	void OnTypeSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnConditionSelect(wxCommandEvent& event);
	void OnImportanceSelect(wxCommandEvent& event);
	void OnAccountsChange(wxListEvent& event);
	void OnNameKillFocus(wxFocusEvent& event);
	void OnMessageKillFocus(wxFocusEvent& event);
	void OnAmountKillFocus(wxFocusEvent& event);	
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};

