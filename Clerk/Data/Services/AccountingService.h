#pragma once

#include <wx/datetime.h>
#include "./AccountsService.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../../Utils/EventEmitter.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class AccountingService {
		public:
			AccountingService(AccountsService& accountsService, ExchangeRatesRepository& exchangeRatesRepository);
			~AccountingService();

			void SetBaseCurrency(int id);
			float GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetBalance();
			float GetCredit();			

			void OnUpdate(std::function<void()> fn);

		private:
			AccountsService& _accountsService;
			ExchangeRatesRepository& _exchangeRatesRepository;
			EventEmitter* _eventEmitter;

			int _baseCurrencyId = 0;
		};
	}
}
