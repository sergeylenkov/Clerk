#include "InterestAccrualViewModel.h"

using namespace Clerk::UI;

InterestAccrualViewModel::InterestAccrualViewModel(AccountsService& accountsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_transactionsService(transactionsService) {
	_id = -1;
	_type = AccrualType::Amount;
	_amount = 0;
	_percent = 0;
	_newBalance = 0;
}

void InterestAccrualViewModel::SetAccountId(int id) {
	auto account = _accountsService.GetById(id);

	if (account) {
		_id = account->id;		
	}
}

void InterestAccrualViewModel::SetType(AccrualType type) {
	_type = type;

	if (OnUpdate) {
		OnUpdate(InterestAccrualViewModelField::Type);
	}
}

AccrualType InterestAccrualViewModel::GetType() {
	return _type;
}

void InterestAccrualViewModel::SetAmount(float amount) {
	_amount = amount;

	CalculateNewBalance();

	if (OnUpdate) {
		OnUpdate(InterestAccrualViewModelField::Amount);
	}
}

float InterestAccrualViewModel::GetAmount() {
	return _amount;
}

void InterestAccrualViewModel::SetPercent(float percent) {
	_percent = percent;

	CalculateNewBalance();

	if (OnUpdate) {
		OnUpdate(InterestAccrualViewModelField::Percent);
	}
}

float InterestAccrualViewModel::GetPercent() {
	return _percent;
}

wxString InterestAccrualViewModel::GetCurrencyName() {
	auto account = _accountsService.GetById(_id);

	if (account) {
		return account->currency->shortName;
	}

	return "";
}

float InterestAccrualViewModel::GetBalance() {
	auto account = _accountsService.GetById(_id);

	if (account) {
		return account->balance;
	}

	return 0.0;
}

float InterestAccrualViewModel::GetNewBalance() {
	return _newBalance;
}

void InterestAccrualViewModel::CalculateNewBalance() {
	_newBalance = GetBalance();

	if (_type == AccrualType::Amount) {
		_newBalance = _newBalance + _amount;
	} else {
		_newBalance += _newBalance * (_percent / 100.0);
	}
}

void InterestAccrualViewModel::Save() {
	auto account = _accountsService.GetById(_id);

	if (account) {
		TransactionPresentationModel* transaction = new TransactionPresentationModel();

		float amount = _amount;

		if (_type == AccrualType::Percent) {
			amount = GetBalance();
			amount += amount * (_percent / 100.0);
		}

		transaction->fromAccount = account;
		transaction->fromAmount = _amount;
		transaction->toAmount = _amount;

		_transactionsService.Save(*transaction);

		delete transaction;
	}
}