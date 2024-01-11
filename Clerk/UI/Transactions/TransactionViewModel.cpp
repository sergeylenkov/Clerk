#include "TransactionViewModel.h"

using namespace Clerk::UI;

TransactionViewModel::TransactionViewModel(TransactionsService& transactionsService, AccountsService& accountsService, CurrenciesService& currenciesService, TagsService& tagsService):
	_transactionsService(transactionsService),
	_accountsService(accountsService),	
	_currenciesService(currenciesService),
	_tagsService(tagsService)
{
	_splitId = -1;
	_id = -1;
	_fromAmount = 0;
	_toAmount = 0;
	_date = wxDateTime::Now();
	
	_subscriptionId = _accountsService.Subscribe([&]() {
		Update();
	});
}

TransactionViewModel::~TransactionViewModel() {
	_accountsService.Unsubscribe(_subscriptionId);
}

void TransactionViewModel::SetTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_id = transaction->id;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = transaction->fromAmount;
		_toAmount = transaction->toAmount;
		_note = transaction->note;
		_tags = transaction->tags;
		_date = transaction->date;

		Update();
	}
}
void TransactionViewModel::SetSplitTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_splitId = transaction->id;
		_id = -1;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = transaction->fromAmount;
		_toAmount = 0;
		_note = transaction->note;
		_date = transaction->date;

		Update();
	}
}

void TransactionViewModel::SetCopyTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_id = -1;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = 0;
		_toAmount = 0;
		_note = transaction->note;
		_tags = transaction->tags;

		Update();
	}
}

void TransactionViewModel::SetAccountId(int id) {
	_fromAccount = _accountsService.GetById(id);

	_id = -1;
	_fromAmount = 0;
	_toAmount = 0;
	_note = "";

	if (_fromAccount) {
		_toAccount = _accountsService.GetPairAccount(*_fromAccount);
	}
	else {
		_fromAccount = _accountsService.GetLastUsedAccount();
		_toAccount = _accountsService.GetPairAccount(*_fromAccount);
	}

	Update();
}

void TransactionViewModel::Update() {
	UpdateFromAccounts();
	UpdateToAccounts();

	if (!_fromAccount) {
		_fromAccount = _fromAccounts[0];
	}

	if (!_toAccount) {
		_toAccount = _toAccounts[0];
	}
}

void TransactionViewModel::UpdateFromAccounts() {
	auto receipts = _accountsService.GetByType(AccountType::Receipt);
	auto deposits = _accountsService.GetByType(AccountType::Deposit);

	std::sort(receipts.begin(), receipts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	std::sort(deposits.begin(), deposits.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_fromAccounts.clear();

	for (auto& account : receipts) {
		if (_toAccount && account->id != _toAccount->id) {
			_fromAccounts.push_back(account);
		}
		else {
			_fromAccounts.push_back(account);
		}
	}
	
	for (auto& account : deposits) {
		if (_toAccount && account->id != _toAccount->id) {
			_fromAccounts.push_back(account);
		}
		else {
			_fromAccounts.push_back(account);
		}
	}
}

void TransactionViewModel::UpdateToAccounts() {
	auto deposits = _accountsService.GetByType(AccountType::Deposit);
	auto virtuals = _accountsService.GetByType(AccountType::Virtual);
	auto expenses = _accountsService.GetByType(AccountType::Expens);
	auto debts = _accountsService.GetByType(AccountType::Debt);

	std::sort(deposits.begin(), deposits.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	std::sort(virtuals.begin(), virtuals.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	std::sort(expenses.begin(), expenses.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	std::sort(debts.begin(), debts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_toAccounts.clear();
	
	shared_vector<AccountPresentationModel> filtered;

	filtered.insert(filtered.end(), deposits.begin(), deposits.end());
	filtered.insert(filtered.end(), virtuals.begin(), virtuals.end());

	if (_fromAccount->type == AccountType::Deposit) {
		filtered.insert(filtered.end(), expenses.begin(), expenses.end());
		filtered.insert(filtered.end(), debts.begin(), debts.end());
	}

	for (auto& account : filtered) {
		if (_fromAccount && account->id != _fromAccount->id) {
			_toAccounts.push_back(account);
		}
		else {
			_toAccounts.push_back(account);
		}
	}
}

shared_vector<AccountPresentationModel> TransactionViewModel::GetFromAccounts() {
	return _fromAccounts;
}

shared_vector<AccountPresentationModel> TransactionViewModel::GetToAccounts() {
	return _toAccounts;
}

int TransactionViewModel::GetFromAccountIndex() {
	for (unsigned int i = 0; i < _fromAccounts.size(); i++) {
		if (_fromAccount->id == _fromAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionViewModel::SetFromAccount(int index) {
	_fromAccount = _fromAccounts[index];

	const auto result = std::find_if(_toAccounts.begin(), _toAccounts.end(), [&](auto account) {
		return account->id == _fromAccount->id;
	});

	if (result != _toAccounts.end()) {
		UpdateToAccounts();

		if (OnUpdate) {
			OnUpdate(TransactionViewModelField::FromAccount);
		}
	}
}

int TransactionViewModel::GetToAccountIndex() {
	for (unsigned int i = 0; i < _toAccounts.size(); i++) {
		if (_toAccount->id == _toAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionViewModel::SetToAccount(int index) {
	_toAccount = _toAccounts[index];

	const auto result = std::find_if(_fromAccounts.begin(), _fromAccounts.end(), [&](auto account) {
		return account->id == _toAccount->id;
	});

	if (result != _fromAccounts.end()) {
		UpdateFromAccounts();

		if (OnUpdate) {
			OnUpdate(TransactionViewModelField::ToAccount);
		}
	}
}

float TransactionViewModel::GetFromAmount() {
	return _fromAmount;
}

void TransactionViewModel::SetFromAmount(float amount) {
	_fromAmount = amount;

	if (_toAmount == 0) {
		float rate = 1;

		if (Settings::GetInstance().IsConvertCurrency()) {
			rate = _currenciesService.GetExchangeRate(*_fromAccount->currency, *_toAccount->currency);
		}

		_toAmount = _fromAmount * rate;
	}

	if (_fromAmount == amount && OnUpdate) {
		OnUpdate(TransactionViewModelField::FromAmount);
	}
}

float TransactionViewModel::GetToAmount() {
	return _toAmount;
}

void TransactionViewModel::SetToAmount(float amount) {
	_toAmount = amount;

	if (_toAmount == amount && OnUpdate) {
		OnUpdate(TransactionViewModelField::ToAmount);
	}
}

void TransactionViewModel::SetNote(wxString note) {
	_note = note;

	if (OnUpdate) {
		OnUpdate(TransactionViewModelField::Note);
	}
}

wxString TransactionViewModel::GetNote() {
	return _note;
}

void TransactionViewModel::SetDate(wxDateTime date) {
	_date = date;

	if (OnUpdate) {
		OnUpdate(TransactionViewModelField::Date);
	}
}

wxDateTime TransactionViewModel::GetDate() {
	return _date;
}

shared_vector<TagPresentationModel> TransactionViewModel::GetTags() {
	return _tags;
}

void TransactionViewModel::SetTags(shared_vector<TagPresentationModel> tags) {
	_tags = tags;

	if (OnUpdate) {
		OnUpdate(TransactionViewModelField::Tags);
	}
}

void TransactionViewModel::Save() {
	auto transaction = _transactionsService.GetById(_id);

	if (!transaction) {
		transaction = std::make_shared<TransactionPresentationModel>();
	}

	transaction->fromAccount = _fromAccount;
	transaction->toAccount = _toAccount;
	transaction->fromAmount = _fromAmount;
	transaction->toAmount = _toAmount;
	transaction->note = _note;
	transaction->tags = _tags;
	transaction->date = _date;
	
	_transactionsService.Save(*transaction);

	auto splitTransaction = _transactionsService.GetById(_splitId);

	if (splitTransaction) {
		_transactionsService.Split(*splitTransaction, *transaction);
	}
}