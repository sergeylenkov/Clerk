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
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../../Data/Settings.h"
#include "../TreeMenu/Enums.h"
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

private:	
	wxDataViewCtrl *_list;
	wxObjectDataPtr<TransactionsListDataModel> _model;
	TransactionsService* _transactionsService;
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
	std::vector<std::shared_ptr<TransactionPresentationModel>> _transactions;
	std::vector<std::shared_ptr<TransactionPresentationModel>> _filtered;
	float balance;
	int sortBy;
	bool sortDesc;
	wxDateTime periodFromDate;
	wxDateTime periodToDate;
	unsigned int _subscriptionId;

	void Update();
	void Sort();
	void Filter();
	void CreateListColumns();
	void UpdateList();
	void UpdateInfo();
	void OnListColumnClick(wxListEvent &event);
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
};