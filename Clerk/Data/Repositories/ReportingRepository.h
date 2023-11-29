#pragma once

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <utility>
#include "BaseRepository.h"

namespace Clerk {
	namespace Data {
		class ReportingRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			std::vector<std::pair<std::string, float>> GetExpensesByMonth(std::string& fromDate, std::string& toDate);
			std::vector<std::pair<std::string, float>> GetExpensesByMonth(int accountId, std::string& fromDate, std::string& toDate);
			std::vector<std::pair<std::string, float>> GetExpensesByMonth(std::string& accountIds, std::string& fromDate, std::string& toDate);
			std::vector<std::pair<std::string, float>> GetExpensesByAccount(std::string& fromDate, std::string& toDate);
			std::vector<std::pair<std::string, float>> GetExpensesByAccount(std::string& accountIds, std::string& fromDate, std::string& toDate);
			std::vector<std::pair<std::string, float>> GetExpensesForAccount(int accountId, std::string& fromDate, std::string& toDate);
		};
	}
}
