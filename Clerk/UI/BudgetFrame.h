#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include "../Data/DataHelper.h"

class BudgetFrame : public wxFrame
{
public:
	BudgetFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~BudgetFrame();

	void SetBudget(std::shared_ptr<Budget> budget);

	std::function<void()> OnClose;

private:
	wxStaticText *nameLabel;
	wxTextCtrl *nameField;
	wxStaticText *typeLabel;
	wxComboBox *typeList;
	wxStaticText *periodLabel;
	wxComboBox *periodList;
	wxStaticText *accountLabel;
	wxBitmapComboBox *accountList;
	wxStaticText *amountLabel;
	wxTextCtrl *amountField;
	wxButton *okButton;
	wxButton *cancelButton;
	float amountValue = 0.0;
	std::shared_ptr<Budget> budget;
	wxImageList *accountsImageList;
	vector<shared_ptr<Account>> accounts;

	void UpdateAccounts();
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};

