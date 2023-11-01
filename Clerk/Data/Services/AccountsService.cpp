#include "AccountsService.h"

AccountsService::AccountsService(AccountsRepository& accountsRepository, CurrenciesService& currenciesService) :
	_accountsRepository(accountsRepository),
	_currenciesService(currenciesService)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

AccountsService::~AccountsService() {
	delete _eventEmitter;
	
	_hash.clear();

	for (auto account = _list.begin(); account != _list.end(); ++account) {
		delete* account;
	}
}

unsigned int AccountsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void AccountsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

AccountPresentationModel* AccountsService::GetById(int id) {
	auto account = GetFromHash(id);

	if (account) {
		return account;
	}

	auto model = _accountsRepository.GetById(id);

	if (model) {
		account = new AccountPresentationModel(*model);
		account->currency = _currenciesService.GetById(model->currencyId);

		AddToHash(account->id, account);

		delete model;

		return account;
	}

	return nullptr;
}

std::vector<AccountPresentationModel*> AccountsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _accountsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto account = new AccountPresentationModel(*model);

			account->currency = _currenciesService.GetById(model->currencyId);
			account->balance = _accountsRepository.GetBalance(account->id, account->type);

			AddToHash(account->id, account);
		}
	}

	for (auto model = models.begin(); model != models.end(); ++model) {
		delete* model;
	}

	models.clear();

	_isLoading = true;

	return GetHashList();
}

std::vector<AccountPresentationModel*> AccountsService::GetActive() {
	if (_active.size() > 0) {
		return _active;
	}

	auto accounts = GetAll();

	for (auto& account : accounts) {
		if (account->isActive) {
			_active.push_back(account);
		}
	}

	return _active;
}

std::vector<AccountPresentationModel*> AccountsService::GetByType(AccountType type) {
	if (_types[type].size() > 0) {
		return _types[type];
	}

	auto accounts = GetActive();

	for (auto& account : accounts) {
		if (account->type == type) {
			_types[type].push_back(account);
		}
	}

	return _types[type];
}

std::vector<AccountPresentationModel*> AccountsService::GetArchive() {
	if (_archive.size() > 0) {
		return _archive;
	}

	auto accounts = GetAll();

	for (auto& account : accounts) {
		if (!account->isActive) {
			_archive.push_back(account);
		}
	}

	return _archive;
}

std::vector<AccountPresentationModel*> AccountsService::GetDeposits() {
	auto accounts = GetByType(AccountType::Deposit);

	std::vector<AccountPresentationModel*> result;

	for (auto& account : accounts) {
		account->balance = _accountsRepository.GetBalance(account->id, account->type);
		result.push_back(account);
	}

	return result;
}

std::vector<AccountPresentationModel*> AccountsService::GetDepositsAndVirtuals() {
	auto accounts = GetByType(AccountType::Deposit);
	auto virtuals = GetByType(AccountType::Virtual);

	accounts.insert(accounts.end(), virtuals.begin(), virtuals.end());

	std::vector<AccountPresentationModel*> result;

	for (auto& account : accounts) {
		account->balance = _accountsRepository.GetBalance(account->id, account->type);
		result.push_back(account);
	}

	return result;
}

std::vector<AccountPresentationModel*> AccountsService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	auto accounts = GetByType(AccountType::Expens);
	auto debts = GetByType(AccountType::Debt);

	accounts.insert(accounts.end(), debts.begin(), debts.end());

	std::vector<AccountPresentationModel*> result;

	for (auto& account : accounts) {
		account->expenses = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

		if (account->expenses > 0) {
			result.push_back(account);
		}
	}

	return result;
}

std::vector<AccountPresentationModel*> AccountsService::GetDebts() {
	auto accounts = GetByType(AccountType::Debt);
	auto credits = GetByType(AccountType::Deposit);

	accounts.insert(accounts.end(), credits.begin(), credits.end());

	std::vector<AccountPresentationModel*> result;

	for (auto& account : accounts) {
		account->expenses = abs(_accountsRepository.GetExpenses(account->id));
		account->receipts = _accountsRepository.GetReceipts(account->id);

		if (account->type == AccountType::Debt) {
			result.push_back(account);
		}
		else if (account->isCredit && account->balance < 0) {
			result.push_back(account);
		}
	}

	return result;
}

AccountPresentationModel* AccountsService::GetPairAccount(const AccountPresentationModel& account) {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetPairAccountId(account.id, account.type, std::string(fromDate.FormatISODate().ToUTF8())));
}

AccountPresentationModel* AccountsService::GetLastUsedAccount() {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetLastUsedAccountId(std::string(fromDate.FormatISODate().ToUTF8())));
}

float AccountsService::GetInitialAmount(const AccountPresentationModel& account) {
	return _accountsRepository.GetInitialAmount(account.id, account.type);
}

AccountPresentationModel* AccountsService::Save(AccountPresentationModel& account) {
	AccountModel& model = account;

	int id = _accountsRepository.Save(model);
	
	delete& model;

	_eventEmitter->Emit();

	return GetById(id);
}

void AccountsService::Delete(AccountPresentationModel& account) {
	AccountModel& model = account;

	_accountsRepository.Delete(model);
	
	delete& model;

	RemoveFromHash(account.id);

	_eventEmitter->Emit();
}