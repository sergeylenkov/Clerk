#pragma once

#include <set>
#include <sstream>
#include <wx/datetime.h>
#include "../Repositories/ReportingRepository.h"
#include "../Services/CurrenciesService.h"
#include "../../UI/PresentationModels/Types.h"
#include "../../UI/PresentationModels/AccountPresentationModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace Data {
		class ReportingService {
		public:
			ReportingService(ReportingRepository& reportingRepository, CurrenciesService& currenciesService);

			std::vector<StringValueViewModel> GetExpensesByAccount(const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<StringValueViewModel> GetExpensesByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<DateValueViewModel> GetExpensesByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<DateValueViewModel> GetBalanceByMonth(const AccountPresentationModel& account, const wxDateTime& fromDate, const wxDateTime& toDate);			
			std::vector<DateValueViewModel> GetReceiptsByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);
			std::vector<StringValueViewModel> GetReceiptsByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate);

		private:
			ReportingRepository& _reportingRepository;
			CurrenciesService& _currenciesService;
		};
	}
}

