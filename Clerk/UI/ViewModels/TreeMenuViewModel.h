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

			std::vector<AccountPresentationModel*> GetReceiptsAccounts();
			std::vector<AccountPresentationModel*> GetDepositsAccounts();
			std::vector<AccountPresentationModel*> GetExpensesAccounts();
			std::vector<AccountPresentationModel*> GetDebtsAccounts();
			std::vector<AccountPresentationModel*> GetVirtualsAccounts();
			std::vector<AccountPresentationModel*> GetArchiveAccounts();
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
