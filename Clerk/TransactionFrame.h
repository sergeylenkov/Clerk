#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/valnum.h>
#include <functional>
#include "DataHelper.h"

using namespace std;

class TransactionFrame : public wxFrame
{
public:
	TransactionFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~TransactionFrame();
	
	void SetAccount(shared_ptr<Account> account);
	void SetTransaction(shared_ptr<Transaction> transaction);
	
	function<void()> OnClose;

private:
	wxStaticText *fromLabel;
	wxStaticText *toLabel;
	wxComboBox *fromList;
	wxComboBox *toList;
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
	vector<shared_ptr<Account>> accounts;
	vector<shared_ptr<Account>> fromAccounts;
	vector<shared_ptr<Account>> toAccounts;
	shared_ptr<Transaction> transaction;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnFromAccountSelect(wxCommandEvent &event);
	void OnToAccountSelect(wxCommandEvent &event);
	void OnFromAmountKillFocus(wxFocusEvent &event);
	void SelectFromAccount(int id);
	void SelectToAccount(int id);
};
