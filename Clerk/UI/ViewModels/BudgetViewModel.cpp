#include "BudgetViewModel.h"

using namespace Clerk::UI;

BudgetViewModel::BudgetViewModel(BudgetsService& budgetsService, AccountsService& accountsService) :
	_budgetsService(budgetsService),
	_accountsService(accountsService)
{
	_id = -1;
	_period = BudgetPeriod::Month;
	_date = wxDateTime::Today();
	_amount = 0;
}

void BudgetViewModel::SetBudgetId(int id) {
	auto alert = _budgetsService.GetById(id);

	if (alert) {
		_id = alert->id;
		_name = alert->name;
		_period = alert->period;
		_date = alert->date;
		_amount = alert->amount;
		_accountsIds = alert->accountsIds;
	}
}

bool BudgetViewModel::IsNew() {
	return _id == -1;
}

void BudgetViewModel::SetName(wxString name) {
	_name = name;

	if (OnUpdate) {
		OnUpdate(BudgetViewModelField::Name);
	}
}

wxString BudgetViewModel::GetName() {
	return _name;
}

void BudgetViewModel::SetPeriod(BudgetPeriod period) {
	_period = period;

	if (OnUpdate) {
		OnUpdate(BudgetViewModelField::Period);
	}
}

BudgetPeriod BudgetViewModel::GetPeriod() {
	return _period;
}

void BudgetViewModel::SetDate(wxDateTime date) {
	_date = date;

	if (OnUpdate) {
		OnUpdate(BudgetViewModelField::Date);
	}
}

wxDateTime BudgetViewModel::GetDate() {
	return _date;
}

void BudgetViewModel::SetAmount(float amount) {
	_amount = amount;

	if (OnUpdate) {
		OnUpdate(BudgetViewModelField::Amount);
	}
}

float BudgetViewModel::GetAmount() {
	return _amount;
}

void BudgetViewModel::SetAccountsIds(std::vector<int> ids) {
	_accountsIds = ids;

	if (OnUpdate) {
		OnUpdate(BudgetViewModelField::Accounts);
	}
}

std::vector<int> BudgetViewModel::GetAccountsIds() {
	return _accountsIds;
}

shared_vector<AccountPresentationModel> BudgetViewModel::GetAccounts() {
	return _accountsService.GetExpenses();
}

void BudgetViewModel::Save() {
	auto budget = _budgetsService.GetById(_id);

	if (!budget) {
		budget = std::make_shared<BudgetPresentationModel>();
	}

	budget->name = _name;
	budget->period = _period;
	budget->date = _date;
	budget->amount = _amount;
	budget->accountsIds = _accountsIds;

	_budgetsService.Save(*budget);
}