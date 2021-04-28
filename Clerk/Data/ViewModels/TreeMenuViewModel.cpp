#include "TreeMenuViewModel.h"

using namespace Clerk::Data;

TreeMenuViewModel::TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_reportsService(reportsService),
	_transactionsService(transactionsService)
{

}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetReceiptsAccounts() {
	return _accountsService.GetByType(AccountType::Receipt);
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetDepositsAccounts() {
	return _accountsService.GetByType(AccountType::Deposit);
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetExpensesAccounts() {
	return _accountsService.GetByType(AccountType::Expens);
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetDebtsAccounts() {
	return _accountsService.GetByType(AccountType::Debt);
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetVirtualsAccounts() {
	return _accountsService.GetByType(AccountType::Virtual);
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetArchiveAccounts() {
	return _accountsService.GetArchive();
}

std::vector<std::shared_ptr<ReportViewModel>> TreeMenuViewModel::GetReports() {
	return _reportsService.GetAll();
}

std::vector<std::shared_ptr<TransactionViewModel>> TreeMenuViewModel::GetRecentsTransactions(const AccountViewModel& account) {
	return _transactionsService.GetRecents(account, 10);
}

bool TreeMenuViewModel::IsTrashEmpty() {
	return _transactionsService.GetDeleted().size() == 0;
}