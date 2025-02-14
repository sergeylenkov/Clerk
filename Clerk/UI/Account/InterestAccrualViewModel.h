#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Utils/Types.h"
#include "Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class InterestAccrualViewModel {
		public:
			InterestAccrualViewModel(AccountsService& accountsService, TransactionsService& transactionsService);

			void SetAccountId(int id);			
			void SetType(AccrualType type);
			AccrualType GetType();
			void SetAmount(float amount);
			float GetAmount();
			void SetPercent(float amount);
			float GetPercent();
			wxString GetCurrencyName();
			float GetBalance();
			float GetNewBalance();

			void Save();
			std::function<void(InterestAccrualViewModelField field)> OnUpdate;

		private:
			AccountsService& _accountsService;
			TransactionsService& _transactionsService;
			int _id;
			AccrualType _type;
			float _amount;
			float _percent;
			float _newBalance;

			void CalculateNewBalance();
		};
	}
}