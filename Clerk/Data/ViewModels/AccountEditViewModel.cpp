#include "AccountEditViewModel.h"

using namespace Clerk::Data;

AccountEditViewModel::AccountEditViewModel(AccountsService& accountsService, CurrenciesRepository& currenciesRepository) :
	_accountsService(accountsService),
	_currenciesRepository(currenciesRepository) {
	_id = -1;
	_amount = 0;
	_type = AccountType::Deposit;
	_currency = currenciesRepository.GetBaseCurrency();
	_currencies = _currenciesRepository.GetAll();

	_types.push_back("Receipt");
	_types.push_back("Deposit");
	_types.push_back("Expens");
	_types.push_back("Debt");
	_types.push_back("Virtual");

	Update();
}

void AccountEditViewModel::SetAccountId(int id) {
	auto account = _accountsService.GetById(id);

	if (account) {
		_id = -1;
		_name = account->name;
		_type = account->type;
		_iconId = account->icon;
		_amount = 0;
		_note = account->note;
		_currency = account->currency;

		Update();
	}
}

void AccountEditViewModel::Update() {
	if (OnUpdate) {
		OnUpdate();
	}
}

std::vector<std::shared_ptr<Currency>> AccountEditViewModel::GetCurrencies() {
	return _currencies;
}

std::vector<wxString> AccountEditViewModel::GetTypes() {
	return _types;
}

void AccountEditViewModel::SetName(wxString name) {
	_name = name;
	Update();
}

wxString AccountEditViewModel::GetName() {
	return _name;
}

void AccountEditViewModel::SetType(AccountType type) {
	_type = type;
	Update();
}

AccountType AccountEditViewModel::GetType() {
	return _type;
}

void AccountEditViewModel::SetAmount(float amount) {
	_amount = amount;
	Update();
}

float AccountEditViewModel::GetAmount() {
	return _amount;
}

void AccountEditViewModel::SetNote(wxString note) {
	_note = note;
	Update();
}

wxString AccountEditViewModel::GetNote() {
	return _note;
}

void AccountEditViewModel::SetCurrency(std::shared_ptr<Currency> currency) {
	_currency = currency;
	Update();
}

std::shared_ptr<Currency> AccountEditViewModel::GetCurrency() {
	return _currency;
}

int AccountEditViewModel::GetCurrencyIndex() {
	for (unsigned int i = 0; i < _currencies.size(); i++) {
		if (_currency->id == _currencies[i]->id) {
			return i;
		}
	}

	return 0;
}

void AccountEditViewModel::SetIconIndex(int index) {
	_iconId = index;
	Update();
}

int AccountEditViewModel::GetIconIndex() {
	return _iconId;
}

void AccountEditViewModel::Save() {
	auto account = _accountsService.GetById(_id);

	if (!account) {
		account = std::make_shared<AccountViewModel>();
	}

	account->name = _name;
	account->type = _type;
	account->currency = _currency;
	account->note = _note;

	_accountsService.Save(*account);
}