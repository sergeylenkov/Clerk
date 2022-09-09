#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/numformatter.h>
#include <functional>
#include <thread>
#include <algorithm>
#include <string> 
#include "../DataPanel.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/Settings.h"
#include "../TreeMenu/Enums.h"
#include "TransactionsListDataModel.h"
#include "TransactionsTagsRender.h"
#include "TransactionsAmountRender.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionsListPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
		Duplicate = 4,
		Split = 5,
		Merge = 6,
		Copy = 7,
	};

	TransactionsListPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker);
	~TransactionsListPanel();

	//void SetAccount(std::shared_ptr<AccountViewModel> account);
	//std::shared_ptr<AccountModel> GetAccount();
	//void SetType(TreeMenuItemTypes type);
	//std::shared_ptr<TransactionViewModel> GetTransaction();	
	void Update();
	//float GetBalance();

	std::function<void()> OnAdd;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnEdit;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnSplit;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnCopy;
	std::function<void()> OnPeriodChanged;	

private:	
	wxDataViewCtrl *list;
	wxObjectDataPtr<TransactionsListDataModel> model;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	wxTextCtrl *searchField;
	wxStaticText *transactionLabel;
	wxStaticText *incomeLabel;
	wxStaticText *outcomeLabel;
	wxPanel *infoPanel;
	std::shared_ptr<AccountModel> account;
	TreeMenuItemType type;
	std::vector<std::shared_ptr<TransactionViewModel>> _transactions;
	std::vector<std::shared_ptr<TransactionViewModel>> _filtered;
	float balance;
	int sortBy;
	bool sortDesc;
	wxDateTime periodFromDate;
	wxDateTime periodToDate;
	
	void Sort();
	void Filter();
	void CreateListColumns();
	void UpdateList();
	void UpdateInfo();
	void Add();
	void Edit();
	void Copy();
	void Delete();
	void Duplicate();
	void Split();
	void Merge();	
	void OnListColumnClick(wxListEvent &event);
	void OnListItemDoubleClick(wxDataViewEvent &event);	
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);	
	void CalculatePeriod();
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void SaveColumnsSettings();
};