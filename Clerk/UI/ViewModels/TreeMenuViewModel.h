#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Data/Services/ReportsService.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../PresentationModels/ReportPresentationModel.h"
#include "../PresentationModels/TransactionPresentationModel.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class TreeMenuViewModel {
		public:
			TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService);

			std::vector<std::shared_ptr<AccountPresentationModel>> GetReceiptsAccounts();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetDepositsAccounts();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetExpensesAccounts();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetDebtsAccounts();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetVirtualsAccounts();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetArchiveAccounts();
			std::vector<std::shared_ptr<ReportPresentationModel>> GetReports();
			std::vector<std::shared_ptr<TransactionPresentationModel>> GetRecentsTransactions(const AccountPresentationModel& account);
			bool IsTrashEmpty();

		private:
			AccountsService& _accountsService;
			TransactionsService& _transactionsService;
			ReportsService& _reportsService;
		};
	}
}
