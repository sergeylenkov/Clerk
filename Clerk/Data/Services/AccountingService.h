#pragma once

#include <wx/datetime.h>
#include "../../Utils/EventEmitter/EventEmitter.h"
#include "AccountsService.h"
#include "CurrenciesService.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace Data {
		class AccountingService {
		public:
			AccountingService(AccountsService& accountsService, CurrenciesService& currenciesService);
			~AccountingService();

			float GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate);
			float GetBalance();
			float GetCredit();			

			void OnUpdate(std::function<void()> fn);

		private:
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;
			EventEmitter* _eventEmitter;
		};
	}
}
