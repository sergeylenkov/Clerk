#pragma once

#include "../Repositories/AccountsRepository.h"
#include "./CurrenciesService.h"
#include "../../UI/PresentationModels/AccountPresentationModel.h"
#include "../../Utils/EventEmitter.h"
#include "./HashService.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class AccountsService : HashService<AccountPresentationModel> {
		public:
			AccountsService(AccountsRepository& accountsRepository, CurrenciesService& currenciesService);
			~AccountsService();

			std::shared_ptr<AccountPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<AccountPresentationModel>> GetAll();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetActive();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetByType(AccountType type);
			std::vector<std::shared_ptr<AccountPresentationModel>> GetArchive();
			std::vector<std::shared_ptr<AccountPresentationModel>> GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<std::shared_ptr<AccountPresentationModel>> GetDebts();
			std::shared_ptr<AccountPresentationModel> GetPairAccount(const AccountPresentationModel& account);
			std::shared_ptr<AccountPresentationModel> GetLastUsedAccount();
			float GetInitialAmount(const AccountPresentationModel& account);

			std::shared_ptr<AccountPresentationModel> Save(AccountPresentationModel& account);
			void Delete(AccountPresentationModel& account);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			AccountsRepository& _accountsRepository;
			CurrenciesService& _currenciesService;
			EventEmitter* _eventEmitter;
		};
	}
}

