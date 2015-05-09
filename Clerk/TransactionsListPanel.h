#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <functional>
#include "DataHelper.h"

enum {
	ID_AddTransaction = 10,
	ID_EditTransaction = 11,
	ID_DeleteTransaction = 12,
	ID_DublicateTransaction = 13
};

class TransactionsListPanel : public wxPanel
{
public:
	TransactionsListPanel(wxWindow *parent, wxWindowID id);
	~TransactionsListPanel();

	void SetAccount(Account *account);
	Transaction *GetTransaction();
	void Update();
	void EditTransaction();
	void DeleteTransaction();
	void DublicateTransaction();
	wxDateTime GetFromDate();
	wxDateTime GetToDate();

	std::function<void()> OnEditTransaction;
	std::function<void()> OnPeriodChanged;

private:
	wxListCtrl *transactionsList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	Account *account;

	void OnListItemClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void CalculatePeriod();
};