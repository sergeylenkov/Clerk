#include "TreeMenuViewModel.h"

using namespace Clerk::UI;

TreeMenuViewModel::TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_reportsService(reportsService),
	_transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();

	_subscriptionId = _accountsService.Subscribe([&]() {
		_eventEmitter->Emit();
	});
}

TreeMenuViewModel::~TreeMenuViewModel() {
	_accountsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void TreeMenuViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetReceiptsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Receipt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetDepositsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Deposit);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetExpensesAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Expens);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetDebtsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Debt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetVirtualsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Virtual);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

shared_vector<AccountPresentationModel> TreeMenuViewModel::GetArchiveAccounts() {
	return _accountsService.GetArchive();
}

shared_vector<ReportPresentationModel> TreeMenuViewModel::GetReports() {
	return _reportsService.GetAll();
}

shared_vector<TransactionPresentationModel> TreeMenuViewModel::GetRecentsTransactions(const AccountPresentationModel& account) {
	return _transactionsService.GetRecents(account, 10);
}

bool TreeMenuViewModel::IsTrashEmpty() {
	return _transactionsService.GetDeleted().size() == 0;
}