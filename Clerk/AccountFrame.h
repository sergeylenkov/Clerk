#include <wx/wx.h>
#include <wx/valnum.h>
#include <functional>
#include "DataHelper.h"

class AccountFrame : public wxFrame
{
public:
	AccountFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AccountFrame();

	void SetAccount(std::shared_ptr<Account>);

	std::function<void()> OnClose;

private:
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *currencyLabel;
	wxComboBox *currencyList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxStaticText *noteLabel;
	wxTextCtrl *noteField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<Account> account;
	vector<std::shared_ptr<Currency>> currencies;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};