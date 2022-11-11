#pragma once

#include <wx/numformatter.h>
#include "../Services/AccountingService.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../Repositories/CurrenciesRepository.h"
#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		class StatusViewModel {
		public:
			StatusViewModel(AccountingService& accountingService, ExchangeRatesRepository& exchangeRatesRepository, CurrenciesRepository& currenciesRepository, std::vector<int> selectedRates);
			~StatusViewModel();

			float GetBalance();
			float GetReceipts();
			float GetExpenses();
			wxString GetExchangeRates();
			std::shared_ptr<Currency> GetBaseCurrency();

			void OnUpdate(std::function<void()> fn);

		private:
			AccountingService& _accountingService;
			ExchangeRatesRepository& _exchangeRatesRepository;
			CurrenciesRepository& _currenciesRepository;
			std::vector<int> _selectedRates;
			EventEmitter* _eventEmitter;
		};
	}
}