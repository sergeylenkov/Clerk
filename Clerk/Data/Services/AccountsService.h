#pragma once

#include "../../Utils/EventEmitter/EventEmitter.h"
#include "../../Utils/Types.h"
#include "../Repositories/AccountsRepository.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "HashService.h"
#include "CurrenciesService.h"

using namespace Clerk::Utils;

namespace Clerk {
	namespace Data {
		class AccountsService : HashService<std::shared_ptr<AccountPresentationModel>> {
		public:
			AccountsService(AccountsRepository& accountsRepository, CurrenciesService& currenciesService);
			~AccountsService();

			std::shared_ptr<AccountPresentationModel> GetById(int id);
			shared_vector<AccountPresentationModel> GetAll();
			shared_vector<AccountPresentationModel> GetActive();
			shared_vector<AccountPresentationModel> GetByType(AccountType type);
			shared_vector<AccountPresentationModel> GetArchive();
			shared_vector<AccountPresentationModel> GetDeposits();
			shared_vector<AccountPresentationModel> GetDepositsAndVirtuals();
			shared_vector<AccountPresentationModel> GetReceipts();
			shared_vector<AccountPresentationModel> GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate);
			shared_vector<AccountPresentationModel> AccountsService::GetExpenses();
			shared_vector<AccountPresentationModel> GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			shared_vector<AccountPresentationModel> GetDebts();
			std::shared_ptr<AccountPresentationModel> GetPairAccount(const AccountPresentationModel& account);
			std::shared_ptr<AccountPresentationModel> GetLastUsedAccount();
			float GetInitialAmount(const AccountPresentationModel& account);
			void Archive(AccountPresentationModel& account);
			void Restore(AccountPresentationModel& account);

			std::shared_ptr<AccountPresentationModel> Save(AccountPresentationModel& account);
			void Delete(AccountPresentationModel& account);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			AccountsRepository& _accountsRepository;
			CurrenciesService& _currenciesService;
			EventEmitter* _eventEmitter;

			shared_vector<AccountPresentationModel> _active;
			shared_vector<AccountPresentationModel> _archive;
			std::map<AccountType, shared_vector<AccountPresentationModel>> _types;
		};
	}
}

