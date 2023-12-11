#pragma once

namespace Clerk {
	namespace Data {
		enum class AccountType {
			Receipt = 0,
			Deposit = 1,
			Expens = 2,
			Debt = 3,
			Virtual = 4,
		};

		enum class BudgetPeriod {
			Week = 0,
			Month = 1,
			Year = 2,
			Custom = 3
		};

		enum class SchedulerType {
			Daily = 0,
			Weekly = 1,
			Monthly = 2,
			Yearly = 3
		};

		enum class AlertType {
			Receipt = 0,
			Expense = 1,
			Balance = 2
		};

		enum class AlertPeriod {
			Week = 0,
			Month = 1,
			Year = 2,
		};

		enum class AlertCondition {
			Less = 0,
			More = 1,
			Equal = 2
		};

		enum class AlertImportance {
			Low = 0,
			Medium = 1,
			High = 2
		};
	}
}
