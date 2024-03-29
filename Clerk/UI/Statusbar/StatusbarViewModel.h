#pragma once

#include "../../Utils/EventEmitter/EventEmitter.h"
#include "../../Utils/Utils.h"
#include "../../Utils/Settings/Settings.h"
#include "../../Data/Services/AccountingService.h"
#include "../../Data/Services/CurrenciesService.h"

using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class StatusbarViewModel {
		public:
			StatusbarViewModel(AccountingService& accountingService, CurrenciesService& currenciesService, std::vector<int> selectedRates);
			~StatusbarViewModel();

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