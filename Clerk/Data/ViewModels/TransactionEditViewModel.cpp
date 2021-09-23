#include "TransactionEditViewModel.h"

using namespace Clerk::Data;

TransactionEditViewModel::TransactionEditViewModel(AccountsService& accountsService): _accountsService(accountsService) {
	Update();
}

void TransactionEditViewModel::Update() {
	auto receipts = _accountsService.GetByType(AccountType::Receipt);
	auto deposits = _accountsService.GetByType(AccountType::Deposit);

	_fromAccounts.clear();
	_fromAccounts.insert(_fromAccounts.end(), receipts.begin(), receipts.end());
	_fromAccounts.insert(_fromAccounts.end(), deposits.begin(), deposits.end());
	
	if (!_fromAccount) {
		_fromAccount = _fromAccounts[0];
	}

	auto virtuals = _accountsService.GetByType(AccountType::Virtual);
	auto expenses = _accountsService.GetByType(AccountType::Expens);
	auto debts = _accountsService.GetByType(AccountType::Debt);

	_toAccounts.clear();

	std::vector<std::shared_ptr<AccountViewModel>> filtered;

	filtered.insert(filtered.end(), deposits.begin(), deposits.end());
	filtered.insert(filtered.end(), virtuals.begin(), virtuals.end());

	if (_fromAccount->type == AccountType::Deposit) {
		filtered.insert(filtered.end(), expenses.begin(), expenses.end());
		filtered.insert(filtered.end(), debts.begin(), debts.end());
	}	

	std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(_toAccounts), [=](const std::shared_ptr<AccountViewModel>& account) {
		return account->id != _fromAccount->id;
	});

	if (!_toAccount) {
		_toAccount = _toAccounts[0];
	}

	if (OnUpdate) {
		OnUpdate();
	}
}

std::vector<std::shared_ptr<AccountViewModel>> TransactionEditViewModel::GetFromAccounts() {	
	return _fromAccounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TransactionEditViewModel::GetToAccounts() {
	return _toAccounts;
}

int TransactionEditViewModel::GetFromAccountIndex() {
	for (unsigned int i = 0; i < _fromAccounts.size(); i++) {
		if (_fromAccount->id == _fromAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionEditViewModel::SetFromAccount(int index) {
	_fromAccount = _fromAccounts[index];
	Update();
}

int TransactionEditViewModel::GetToAccountIndex() {
	for (unsigned int i = 0; i < _toAccounts.size(); i++) {
		if (_toAccount->id == _toAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionEditViewModel::SetToAccount(int index) {
	_toAccount = _toAccounts[index];
	Update();
}