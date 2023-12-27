#include "GoalViewModel.h"

using namespace Clerk::UI;

GoalViewModel::GoalViewModel(GoalsService& goalsService, AccountsService& accountsService) :
	_goalsService(goalsService),
	_accountsService(accountsService)
{
	_id = -1;
	_period = BudgetPeriod::Month;
	_date = wxDateTime::Today();
	_amount = 0;
}

void GoalViewModel::SetGoalId(int id) {
	auto goal = _goalsService.GetById(id);

	if (goal) {
		_id = goal->id;
		_name = goal->name;
		_date = goal->date;
		_amount = goal->amount;
		_accountsIds = goal->accountsIds;
	}
}

bool GoalViewModel::IsNew() {
	return _id == -1;
}

void GoalViewModel::SetName(wxString name) {
	_name = name;

	if (OnUpdate) {
		OnUpdate(GoalViewModelField::Name);
	}
}

wxString GoalViewModel::GetName() {
	return _name;
}

void GoalViewModel::SetDate(wxDateTime date) {
	_date = date;

	if (OnUpdate) {
		OnUpdate(GoalViewModelField::Date);
	}
}

wxDateTime GoalViewModel::GetDate() {
	return _date;
}

void GoalViewModel::SetAmount(float amount) {
	_amount = amount;

	if (OnUpdate) {
		OnUpdate(GoalViewModelField::Amount);
	}
}

float GoalViewModel::GetAmount() {
	return _amount;
}

void GoalViewModel::SetAccountsIds(std::vector<int> ids) {
	_accountsIds = ids;

	if (OnUpdate) {
		OnUpdate(GoalViewModelField::Accounts);
	}
}

std::vector<int> GoalViewModel::GetAccountsIds() {
	return _accountsIds;
}

shared_vector<AccountPresentationModel> GoalViewModel::GetAccounts() {
	return _accountsService.GetDepositsAndVirtuals();
}

void GoalViewModel::Save() {
	auto goal = _goalsService.GetById(_id);

	if (!goal) {
		goal = std::make_shared<GoalPresentationModel>();
	}

	goal->name = _name;
	goal->date = _date;
	goal->amount = _amount;
	goal->accountsIds = _accountsIds;

	_goalsService.Save(*goal);
}