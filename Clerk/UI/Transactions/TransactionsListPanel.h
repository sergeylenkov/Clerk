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
#include <optional> 
#include "../DataPanel.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../../Data/Settings.h"
#include "../../Utils/Types.h"
#include "./TransactionsListDataModel.h"
#include "./TransactionsTagsRender.h"
#include "./TransactionsAmountRender.h"
#include "./TransactionContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionsListPanel : public DataPanel
{
public:
	TransactionsListPanel(wxWindow *parent, DataContext& context);
	~TransactionsListPanel();

	void Update();
	void SetAccount(std::shared_ptr<AccountPresentationModel> account);
	void SetAccountType(AccountType type);

private:	
	wxDataViewCtrl *_list;
	wxObjectDataPtr<TransactionsListDataModel> _model;
	TransactionsService* _transactionsService;
	wxComboBox *_periodList;
	wxDatePickerCtrl *_fromDatePicker;
	wxDatePickerCtrl *_toDatePicker;
	wxTextCtrl *_searchField;
	wxStaticText *_transactionLabel;
	wxStaticText *_incomeLabel;
	wxStaticText *_outcomeLabel;
	wxPanel *_infoPanel;
	std::shared_ptr<AccountPresentationModel> _account;
	std::optional<AccountType> _accountType;
	shared_vector<TransactionPresentationModel> _transactions;
	shared_vector<TransactionPresentationModel> _filtered;
	float _balance;
	TransactionsListColumns _sortBy;
	bool _sortDesc;
	wxDateTime _periodFromDate;
	wxDateTime _periodToDate;
	unsigned int _subscriptionId;		
	
	void Filter();
	void CreateListColumns();
	void UpdateList();
	void UpdateInfo();
	void OnListColumnClick(wxDataViewEvent &event);
	void OnListItemDoubleClick(wxDataViewEvent &event);	
	void OnRightClick(wxDataViewEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);	
	void CalculatePeriod();
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void SaveColumnsSettings();
	std::shared_ptr<TransactionPresentationModel> GetTransaction();
	std::vector<int> GetSelectedIds();
	TransactionsListType GetListType();
};