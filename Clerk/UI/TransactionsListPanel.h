#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <functional>
#include "DataPanel.h"
#include "../Data/DataHelper.h"
#include "../TreeMenuItemData.h"
#include "../Settings.h"

enum {
	ID_AddTransaction = 10,
	ID_EditTransaction = 11,
	ID_DeleteTransaction = 12,
	ID_DublicateTransaction = 13,
	ID_SplitTransaction = 14,
	ID_MergeTransaction = 15,
};

class TransactionsListPanel : public DataPanel
{
public:
	TransactionsListPanel(wxWindow *parent, wxWindowID id);
	~TransactionsListPanel();

	void SetAccount(std::shared_ptr<Account> account);
	std::shared_ptr<Account> GetAccount();
	void SetType(TreeMenuItemTypes type);
	std::shared_ptr<Transaction> GetTransaction();
	void Update();
	void EditTransaction();
	void DeleteTransaction();
	void DublicateTransaction();
	void SplitTransaction();
	void MergeTransactions();
	wxDateTime GetFromDate();
	wxDateTime GetToDate();
	float GetBalance();

	std::function<void(std::shared_ptr<Transaction>)> OnEditTransaction;
	std::function<void(std::shared_ptr<Transaction>)> OnSplitTransaction;
	std::function<void()> OnPeriodChanged;	

private:
	wxListCtrl *transactionsList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	wxTextCtrl *searchField;
	wxStaticText *transactionLabel;
	wxStaticText *incomeLabel;
	wxStaticText *outcomeLabel;
	wxPanel *infoPanel;
	std::shared_ptr<Account> account;
	TreeMenuItemTypes type;
	std::vector<std::shared_ptr<Transaction>> transactions;
	float balance;
	int sortBy;
	bool sortDesc;

	void OnListItemClick(wxListEvent &event);
	void OnListColumnClick(wxListEvent &event);
	void OnListItemDoubleClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);	
	void CalculatePeriod();
};