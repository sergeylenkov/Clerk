#include "AccountsService.h"

AccountsService::AccountsService(AccountsRepository& accountsRepository, CurrenciesRepository& currenciesRepository) :
	_accountsRepository(accountsRepository),
	_currenciesRepository(currenciesRepository)
{
	_eventEmitter = new EventEmitter();
}

AccountsService::~AccountsService() {
	delete _eventEmitter;
}

void AccountsService::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

std::shared_ptr<AccountViewModel> AccountsService::GetById(int id) {
	auto account = _accountsRepository.GetById(id);

	if (account) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);

		return model;
	}

	return nullptr;
}

std::vector<std::shared_ptr<AccountViewModel>> AccountsService::GetActive() {
	auto accounts = _accountsRepository.GetAll();
	std::vector<std::shared_ptr<AccountModel>> filtered;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(filtered), [](const std::shared_ptr<AccountModel>& account) {
		return account->isActive;
	});

	std::vector<std::shared_ptr<AccountViewModel>> result;

	std::transform(filtered.begin(), filtered.end(), std::back_inserter(result), [&](const std::shared_ptr<AccountModel>& account) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<AccountViewModel>> AccountsService::GetByType(AccountType type) {
	auto accounts = _accountsRepository.GetByType(type);

	std::vector<std::shared_ptr<AccountViewModel>> result;

	std::transform(accounts.begin(), accounts.end(), std::back_inserter(result), [&](const std::shared_ptr<AccountModel>& account) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);
		
		if (type == AccountType::Deposit || type == AccountType::Virtual) {
			model->balance = _accountsRepository.GetBalance(account->id, account->type);
		}

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<AccountViewModel>> AccountsService::GetArchive() {
	auto accounts = _accountsRepository.GetAll();

	std::vector<std::shared_ptr<AccountModel>> filtered;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(filtered), [](const std::shared_ptr<AccountModel>& account) {
		return !account->isActive;
	});

	std::vector<std::shared_ptr<AccountViewModel>> result;

	std::transform(filtered.begin(), filtered.end(), std::back_inserter(result), [&](const std::shared_ptr<AccountModel>& account) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<AccountViewModel>> AccountsService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	auto accounts = _accountsRepository.GetByType(AccountType::Expens);
	auto debts = _accountsRepository.GetByType(AccountType::Debt);

	accounts.insert(accounts.end(), debts.begin(), debts.end());

	std::vector<std::shared_ptr<AccountViewModel>> result;

	for (auto& account : accounts) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);

		model->balance = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

		if (model->balance > 0) {
			result.push_back(model);
		}
	}

	return result;
}

std::vector<std::shared_ptr<AccountViewModel>> AccountsService::GetDebts() {
	auto accounts = _accountsRepository.GetByType(AccountType::Debt);
	auto credits = _accountsRepository.GetByType(AccountType::Deposit);

	accounts.insert(accounts.end(), credits.begin(), credits.end());

	std::vector<std::shared_ptr<AccountViewModel>> result;

	for (auto& account : accounts) {
		auto model = std::make_shared<AccountViewModel>(*account);
		model->currency = _currenciesRepository.GetById(account->currencyId);

		model->balance = _accountsRepository.GetBalance(account->id, account->type);
		model->expenses = abs(_accountsRepository.GetExpenses(account->id));
		model->receipts = _accountsRepository.GetReceipts(account->id);

		if (model->type == AccountType::Debt) {
			result.push_back(model);
		}
		else if (model->isCredit && model->balance < 0) {
			result.push_back(model);
		}
	}

	return result;
}

std::shared_ptr<AccountViewModel> AccountsService::GetPairAccount(const AccountViewModel& account) {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetPairAccountId(account.id, account.type, std::string(fromDate.FormatISODate().ToUTF8())));
}

std::shared_ptr<AccountViewModel> AccountsService::GetLastUsedAccount() {
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	return GetById(_accountsRepository.GetLastUsedAccountId(std::string(fromDate.FormatISODate().ToUTF8())));
}

void AccountsService::Save(AccountViewModel& viewModel) {
	_eventEmitter->Emit();
}

void AccountsService::Delete(AccountViewModel& viewModel) {
	AccountModel& model = viewModel;

	_accountsRepository.Delete(model);

	delete& model;

	_eventEmitter->Emit();
}