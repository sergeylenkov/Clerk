#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <functional>
#include <thread>
#include <algorithm>
#include <string> 
#include <optional> 
#include "../../Data/PresentationModels/TransactionPresentationModel.h"
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "../DataListPanel.h"
#include "TransactionsListDataModel.h"
#include "TransactionsTagsRender.h"
#include "TransactionsAmountRender.h"
#include "TransactionContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionsListPanel : public DataListPanel
{
public:
	TransactionsListPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~TransactionsListPanel();

	void Update();
	void SetAccount(std::shared_ptr<AccountPresentationModel> account);
	void SetAccountType(AccountType type);

private:
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
	wxDateTime _periodFromDate;
	wxDateTime _periodToDate;
	unsigned int _subscriptionId;		
	
	void CreateListColumns() override;
	void SaveColumnsSettings() override;
	void ShowContextMenu() override;
	void EditSelectedItem() override;
	void Filter();
	void UpdateList();
	void UpdateInfo();
	void OnPeriodSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnSearchChanged(wxCommandEvent &event);	
	void CalculatePeriod();
	void RestoreFilterSettings();
	void SaveFilterSettings();
	std::shared_ptr<TransactionPresentationModel> GetTransaction();
	std::vector<int> GetSelectedIds();
	TransactionsListType GetListType();
};