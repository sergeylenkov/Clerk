#pragma once

#include <wx/datetime.h>
#include "../Repositories/AccountsRepository.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../../Utils/EventEmitter.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class AccountingService {
		public:
			AccountingService(AccountsRepository& accountsRepository, ExchangeRatesRepository& exchangeRatesRepository);
			~AccountingService();

			void SetBaseCurrency(int id);
			float GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetBalance();
			float GetCredit();			

			void OnUpdate(std::function<void()> fn);

		private:
			AccountsRepository& _accountsRepository;
			ExchangeRatesRepository& _exchangeRatesRepository;
			EventEmitter* _eventEmitter;

			int _baseCurrencyId = 0;
		};
	}
}
