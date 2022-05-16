#pragma once

#include "../Repositories/AccountsRepository.h"
#include "../Repositories/CurrenciesRepository.h"
#include "../ViewModels/AccountViewModel.h"
#include "../../Utils/EventEmitter.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class AccountsService {
		public:
			AccountsService(AccountsRepository& accountsRepository, CurrenciesRepository& currenciesRepository);
			~AccountsService();

			std::shared_ptr<AccountViewModel> GetById(int id);
			std::vector<std::shared_ptr<AccountViewModel>> GetActive();
			std::vector<std::shared_ptr<AccountViewModel>> GetByType(AccountType type);
			std::vector<std::shared_ptr<AccountViewModel>> GetArchive();
			std::vector<std::shared_ptr<AccountViewModel>> GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<std::shared_ptr<AccountViewModel>> GetDebts();
			std::shared_ptr<AccountViewModel> GetPairAccount(const AccountViewModel& account);
			std::shared_ptr<AccountViewModel> GetLastUsedAccount();

			void Save(AccountViewModel& viewModel);
			void Delete(AccountViewModel& viewModel);

			void OnUpdate(std::function<void()> fn);

		private:
			AccountsRepository& _accountsRepository;
			CurrenciesRepository& _currenciesRepository;
			EventEmitter* _eventEmitter;
		};
	}
}

