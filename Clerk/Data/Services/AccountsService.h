#pragma once

#include "../Repositories/AccountsRepository.h"
#include "./CurrenciesService.h"
#include "../../UI/PresentationModels/AccountPresentationModel.h"
#include "../../Utils/EventEmitter.h"
#include "./HashService.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class AccountsService : HashService<AccountPresentationModel*> {
		public:
			AccountsService(AccountsRepository& accountsRepository, CurrenciesService& currenciesService);
			~AccountsService();

			AccountPresentationModel* GetById(int id);
			std::vector<AccountPresentationModel*> GetAll();
			std::vector<AccountPresentationModel*> GetActive();
			std::vector<AccountPresentationModel*> GetByType(AccountType type);
			std::vector<AccountPresentationModel*> GetArchive();
			std::vector<AccountPresentationModel*> GetDeposits();
			std::vector<AccountPresentationModel*> GetDepositsAndVirtuals();
			std::vector<AccountPresentationModel*> GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<AccountPresentationModel*> GetDebts();
			AccountPresentationModel* GetPairAccount(const AccountPresentationModel& account);
			AccountPresentationModel* GetLastUsedAccount();
			float GetInitialAmount(const AccountPresentationModel& account);

			AccountPresentationModel* Save(AccountPresentationModel& account);
			void Delete(AccountPresentationModel& account);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			AccountsRepository& _accountsRepository;
			CurrenciesService& _currenciesService;
			EventEmitter* _eventEmitter;

			std::vector<AccountPresentationModel*> _active;
			std::vector<AccountPresentationModel*> _archive;
			std::map<AccountType, std::vector<AccountPresentationModel*>> _types;
			boolean _isLoading;
		};
	}
}

