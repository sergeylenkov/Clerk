#include "AccountViewModel.h"

using namespace Clerk::UI;

AccountViewModel::AccountViewModel(AccountsService& accountsService, CurrenciesService& currenciesService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_currenciesService(currenciesService),
	_transactionsService(transactionsService) {
	_id = -1;
	_initialAmount = 0;
	_type = AccountType::Deposit;
	_currency = _currenciesService.GetBaseCurrency();
	_currencies = _currenciesService.GetAll();
	_creditLimit = 0;
	_iconId = -1;

	_types.push_back(_("Receipt"));
	_types.push_back(_("Deposit"));
	_types.push_back(_("Expense"));
	_types.push_back(_("Debt"));
	_types.push_back(_("Virtual"));
}

void AccountViewModel::SetAccountId(int id) {
	auto account = _accountsService.GetById(id);

	if (account) {
		_id = account->id;
		_name = account->name;
		_type = account->type;
		_iconId = account->icon;
		_initialAmount = _accountsService.GetInitialAmount(*account);
		_note = account->note;
		_currency = account->currency;
		_creditLimit = account->creditLimit;
	}
}

void AccountViewModel::SetAccountType(AccountType type) {
	_type = type;
}

bool AccountViewModel::IsNew() {
	return _id == -1;
}

shared_vector<CurrencyPresentationModel> AccountViewModel::GetCurrencies() {
	return _currencies;
}

std::vector<wxString> AccountViewModel::GetTypes() {
	return _types;
}

void AccountViewModel::SetName(wxString name) {
	_name = name;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::Name);
	}
}

wxString AccountViewModel::GetName() {
	return _name;
}

void AccountViewModel::SetType(AccountType type) {
	_type = type;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::Type);
	}
}

AccountType AccountViewModel::GetType() {
	return _type;
}

void AccountViewModel::SetInitialAmount(float amount) {
	_initialAmount = amount;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::InitialAmount);
	}
}

float AccountViewModel::GetInitialAmount() {
	return _initialAmount;
}

void AccountViewModel::SetCreditLimit(float amount) {
	_creditLimit = amount;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::CreditLimit);
	}
}

float AccountViewModel::GetCreditLimit() {
	return _creditLimit;
}

void AccountViewModel::SetNote(wxString note) {
	_note = note;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::Note);
	}
}

wxString AccountViewModel::GetNote() {
	return _note;
}

void AccountViewModel::SetCurrency(std::shared_ptr<CurrencyPresentationModel> currency) {
	_currency = currency;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::Currency);
	}
}

std::shared_ptr<CurrencyPresentationModel> AccountViewModel::GetCurrency() {
	return _currency;
}

int AccountViewModel::GetCurrencyIndex() {
	for (unsigned int i = 0; i < _currencies.size(); i++) {
		if (_currency->id == _currencies[i]->id) {
			return i;
		}
	}

	return 0;
}

void AccountViewModel::SetIconIndex(int index) {
	_iconId = index;

	if (OnUpdate) {
		OnUpdate(AccountViewModelField::Icon);
	}
}

int AccountViewModel::GetIconIndex() {
	return _iconId;
}

void AccountViewModel::Save() {
	auto account = _accountsService.GetById(_id);

	if (!account) {
		account = std::make_shared<AccountPresentationModel>();
	}

	account->name = _name;
	account->type = _type;
	account->currency = _currency;
	account->creditLimit = _creditLimit;
	account->icon = _iconId;
	account->note = _note;

	auto newAccount = _accountsService.Save(*account);

	if (IsNew() && _initialAmount > 0) {
		TransactionPresentationModel* transaction = new TransactionPresentationModel();

		if (account->type == AccountType::Debt) {
			transaction->fromAccount = newAccount;
			transaction->fromAmount = _initialAmount;
			transaction->toAmount = _initialAmount;
		}
		else {
			transaction->toAccount = newAccount;
			transaction->fromAmount = _initialAmount;
			transaction->toAmount = _initialAmount;
		}

		_transactionsService.Save(*transaction);

		delete transaction;
	}
}