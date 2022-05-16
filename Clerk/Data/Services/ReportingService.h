#pragma once

#include <set>
#include <wx/datetime.h>
#include "../Repositories/ReportingRepository.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../ViewModels/StringValueViewModel.h"
#include "../ViewModels/DateValueViewModel.h"
#include "../ViewModels/AccountViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class ReportingService {
		public:
			ReportingService(ReportingRepository& reportingRepository, ExchangeRatesRepository& exchangeRatesRepository);

			void SetBaseCurrency(int id);
			std::vector<StringValueViewModel> GetExpensesByAccount(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<StringValueViewModel> GetExpensesByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<DateValueViewModel> GetExpensesByMonth(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<DateValueViewModel> GetExpensesByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<DateValueViewModel> GetBalanceByMonth(const AccountViewModel& account, const wxDateTime& fromDate, const wxDateTime& toDate);

		private:
			ReportingRepository& _reportingRepository;
			ExchangeRatesRepository& _exchangeRatesRepository;

			int _baseCurrencyId = 0;
		};
	}
}

