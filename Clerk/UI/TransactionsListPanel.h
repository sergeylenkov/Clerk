#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/numformatter.h>
#include <functional>
#include <thread>
#include "DataPanel.h"
#include "../Data/DataHelper.h"
#include "../TreeMenuItemData.h"
#include "../Settings.h"

enum class TransactionsPanelMenuTypes {
	Add = 1,
	Edit = 2,
	Delete = 3,
	Duplicate = 4,
	Split = 5,
	Merge = 6,
	Copy = 7,
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
	wxDateTime GetFromDate();
	wxDateTime GetToDate();
	float GetBalance();

	std::function<void()> OnAdd;
	std::function<void(std::shared_ptr<Transaction>)> OnEdit;
	std::function<void(std::shared_ptr<Transaction>)> OnSplit;
	std::function<void(std::shared_ptr<Transaction>)> OnCopy;
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
	std::vector<std::shared_ptr<Transaction>> filtered;
	float balance;
	int sortBy;
	bool sortDesc;
	wxDateTime periodFromDate;
	wxDateTime periodToDate;

	void Sort();
	void Filter();
	void UpdateList();
	void Add();
	void Edit();
	void Copy();
	void Delete();
	void Duplicate();
	void Split();
	void Merge();	
	void OnListColumnClick(wxListEvent &event);
	void OnListItemDoubleClick(wxListEvent &event);
	void OnColumnDragged(wxListEvent &event);
	void OnRightClick(wxContextMenuEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);	
	void CalculatePeriod();
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void SaveColumnsSettings();
};