#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <functional>
#include "Data/DataHelper.h"
#include "TreeMenuItemData.h"
#include "Settings.h"

enum {
	ID_AddTransaction = 10,
	ID_EditTransaction = 11,
	ID_DeleteTransaction = 12,
	ID_DublicateTransaction = 13,
	ID_SplitTransaction = 14,
};

class TransactionsListPanel : public wxPanel
{
public:
	TransactionsListPanel(wxWindow *parent, wxWindowID id);
	~TransactionsListPanel();

	void SetAccount(Account *account);
	void SetType(TreeMenuItemTypes type);
	shared_ptr<Transaction> GetTransaction();
	void Update();
	void EditTransaction();
	void DeleteTransaction();
	void DublicateTransaction();
	void SplitTransaction();
	wxDateTime GetFromDate();
	wxDateTime GetToDate();
	float GetBalance();

	std::function<void()> OnEditTransaction;
	std::function<void()> OnSplitTransaction;
	std::function<void()> OnPeriodChanged;	

private:
	wxListCtrl *transactionsList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	wxTextCtrl *searchField;
	Account *account;
	TreeMenuItemTypes type;
	vector<shared_ptr<Transaction>> transactions;
	float balance;
	int sortBy;
	bool sortDesc;

	void OnListItemClick(wxListEvent &event);
	void OnListColumnClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);
	void CalculatePeriod();
};