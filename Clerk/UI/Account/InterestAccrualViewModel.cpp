#include "InterestAccrualViewModel.h"

using namespace Clerk::UI;

InterestAccrualViewModel::InterestAccrualViewModel(AccountsService& accountsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_transactionsService(transactionsService) {
	_id = -1;
	_amount = 0;	
}

void InterestAccrualViewModel::SetAccountId(int id) {
	auto account = _accountsService.GetById(id);

	if (account) {
		_id = account->id;		
	}
}

void InterestAccrualViewModel::SetAmount(float amount) {
	_amount = amount;

	if (OnUpdate) {
		OnUpdate(InterestAccrualViewModelField::Amount);
	}
}

float InterestAccrualViewModel::GetAmount() {
	return _amount;
}

void InterestAccrualViewModel::Save() {
	auto account = _accountsService.GetById(_id);

	if (account) {
		TransactionPresentationModel* transaction = new TransactionPresentationModel();

		transaction->toAccount = account;
		transaction->fromAmount = _amount;
		transaction->toAmount = _amount;

		_transactionsService.Save(*transaction);

		delete transaction;
	}
}