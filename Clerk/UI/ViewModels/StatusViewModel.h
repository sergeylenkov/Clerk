#pragma once

#include <wx/numformatter.h>
#include "../../Data/Services/AccountingService.h"
#include "../../Data/Services/CurrenciesService.h"
#include "../../UI/PresentationModels/CurrencyPresentationModel.h"

namespace Clerk {
	namespace UI {
		class StatusViewModel {
		public:
			StatusViewModel(AccountingService& accountingService, CurrenciesService& currenciesService, std::vector<int> selectedRates);
			~StatusViewModel();

			float GetBalance();
			float GetReceipts();
			float GetExpenses();
			wxString GetExchangeRates();
			std::shared_ptr<CurrencyPresentationModel> GetBaseCurrency();
			void SetIsExchangeRatesLoading(boolean isLoading);
			boolean GetIsExchangeRatesLoading();
			void OnUpdate(std::function<void()> fn);

		private:
			AccountingService& _accountingService;
			CurrenciesService& _currenciesService;

			std::vector<int> _selectedRates;
			boolean _isExchangeRatesLoading;
			EventEmitter* _eventEmitter;
		};
	}
}