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

			void SetBaseCurrency(int id);
			float GetReceipts(wxDateTime& fromDate, wxDateTime& toDate);
			float GetExpenses(wxDateTime& fromDate, wxDateTime& toDate);
			float GetBalance();
			float GetCredit();			

		private:
			AccountsRepository& _accountsRepository;
			ExchangeRatesRepository& _exchangeRatesRepository;
			EventEmitter* _eventEmitter;

			int _baseCurrencyId = 0;
		};
	}
}
