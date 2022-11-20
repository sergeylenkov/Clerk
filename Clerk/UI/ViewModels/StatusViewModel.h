#pragma once

#include <wx/numformatter.h>
#include "../../Data/Services/AccountingService.h"
#include "../../Data/Services/CurrenciesService.h"
#include "../../Data/Repositories/ExchangeRatesRepository.h"
#include "../../UI/PresentationModels/CurrencyPresentationModel.h"

namespace Clerk {
	namespace UI {
		class StatusViewModel {
		public:
			StatusViewModel(AccountingService& accountingService, ExchangeRatesRepository& exchangeRatesRepository, CurrenciesService& currenciesService, std::vector<int> selectedRates);
			~StatusViewModel();

			float GetBalance();
			float GetReceipts();
			float GetExpenses();
			wxString GetExchangeRates();
			std::shared_ptr<CurrencyPresentationModel> GetBaseCurrency();

			void OnUpdate(std::function<void()> fn);

		private:
			AccountingService& _accountingService;
			ExchangeRatesRepository& _exchangeRatesRepository;
			CurrenciesService& _currenciesService;
			std::vector<int> _selectedRates;
			EventEmitter* _eventEmitter;
		};
	}
}