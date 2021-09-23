#pragma once

#include "./AccountViewModel.h"
#include "../Services/AccountsService.h"

namespace Clerk {
	namespace Data {
		class TransactionEditViewModel {
		public:
			TransactionEditViewModel(AccountsService& accountsService);
			
			std::vector<std::shared_ptr<AccountViewModel>> GetFromAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetToAccounts();
			int GetFromAccountIndex();
			void SetFromAccount(int index);
			int GetToAccountIndex();
			void SetToAccount(int index);

			std::function<void()> OnUpdate;

		private:
			AccountsService& _accountsService;
			std::vector<std::shared_ptr<AccountViewModel>> _fromAccounts;
			std::vector<std::shared_ptr<AccountViewModel>> _toAccounts;
			std::shared_ptr<AccountViewModel> _fromAccount;
			std::shared_ptr<AccountViewModel> _toAccount;

			void Update();
		};
	}
}