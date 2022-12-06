#include "AccountsService.h"

AccountsService::AccountsService(AccountsRepository& accountsRepository, CurrenciesService& currenciesService) :
	_accountsRepository(accountsRepository),
	_currenciesService(currenciesService)
{
	_eventEmitter = new EventEmitter();
}

AccountsService::~AccountsService() {
	delete _eventEmitter;
}

unsigned int AccountsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void AccountsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

std::shared_ptr<AccountPresentationModel> AccountsService::GetById(int id) {
	auto account = GetFromHash(id);

	if (account) {
		return account;
	}

	auto model = _accountsRepository.GetById(id);

	if (model) {
		account = std::make_shared<AccountPresentationModel>(*model);
		account->currency = _currenciesService.GetById(model->currencyId);

		AddToHash(account->id, account);

		return account;
	}

	return nullptr;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetAll() {
	auto models = _accountsRepository.GetAll();

	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	std::transform(models.begin(), models.end(), std::back_inserter(result), [&](const std::shared_ptr<AccountModel>& model) {
		auto account = GetFromHash(model->id);

		if (account) {
			return account;
		}

		account = std::make_shared<AccountPresentationModel>(*model);
		account->currency = _currenciesService.GetById(model->currencyId);

		AddToHash(account->id, account);
		
		return account;
	});

	return result;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetActive() {
	auto accounts = GetAll();
	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [](const std::shared_ptr<AccountPresentationModel>& account) {
		return account->isActive;
	});

	return result;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetByType(AccountType type) {
	auto accounts = GetActive();

	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [&](const std::shared_ptr<AccountPresentationModel>& account) {
		return account->type == type;
	});

	return result;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetArchive() {
	auto accounts = GetAll();
	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [](const std::shared_ptr<AccountPresentationModel>& account) {
		return !account->isActive;
	});

	return result;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	auto accounts = GetByType(AccountType::Expens);
	auto debts = GetByType(AccountType::Debt);

	accounts.insert(accounts.end(), debts.begin(), debts.end());

	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	for (auto& account : accounts) {
		account->balance = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

		if (account->balance > 0) {
			result.push_back(account);
		}
	}

	return result;
}

std::vector<std::shared_ptr<AccountPresentationModel>> AccountsService::GetDebts() {
	auto accounts = GetByType(AccountType::Debt);
	auto credits = GetByType(AccountType::Deposit);

	accounts.insert(accounts.end(), credits.begin(), credits.end());

	std::vector<std::shared_ptr<AccountPresentationModel>> result;

	for (auto& account : accounts) {
		account->balance = _accountsRepository.GetBalance(account->id, account->type);
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

std::shared_ptr<AccountPresentationModel> AccountsService::GetPairAccount(const AccountPresentationModel& account) {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetPairAccountId(account.id, account.type, std::string(fromDate.FormatISODate().ToUTF8())));
}

std::shared_ptr<AccountPresentationModel> AccountsService::GetLastUsedAccount() {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetLastUsedAccountId(std::string(fromDate.FormatISODate().ToUTF8())));
}

float AccountsService::GetInitialAmount(const AccountPresentationModel& account) {
	return _accountsRepository.GetInitialAmount(account.id, account.type);
}

std::shared_ptr<AccountPresentationModel> AccountsService::Save(AccountPresentationModel& account) {
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