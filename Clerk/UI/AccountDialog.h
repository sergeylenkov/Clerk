#include <wx/wx.h>
#include <wx/valnum.h>
#include <wx/bmpcbox.h>
#include <functional>
#include "../Data/DataHelper.h"

class AccountDialog : public wxFrame
{
public:
	AccountDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AccountDialog();

	void SetAccount(std::shared_ptr<Account> account);

	std::function<void()> OnClose;

private:
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *currencyLabel;
	wxComboBox *currencyList;
	wxStaticText *iconLabel;
	wxBitmapComboBox *iconList;
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