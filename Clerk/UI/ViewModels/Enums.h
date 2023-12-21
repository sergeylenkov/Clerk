#pragma once

namespace Clerk {
	namespace Data {
		enum class TransactionViewModelField {
			FromAccount = 0,
			ToAccount = 1,
			FromAmount = 2,
			ToAmount = 3,
			Date = 4,
			Tags = 5,
			Note = 6,
		};

		enum class AccountViewModelField {
			Name = 0,
			Type = 1,
			Icon = 2,
			Amount = 3,			
			Note = 4,
			Currency = 5,
			CreditLimit = 6,
		};

		enum class AlertViewModelField {
			Name = 0,
			Message = 1,
			Type = 2,
			Period = 3,
			Condition = 4,
			Importance = 5,
			Amount = 6,
			Accounts = 7
		};

		enum class BudgetViewModelField {
			Name = 0,
			Period = 1,
			Date = 2,
			Amount = 3,
			Accounts = 4
		};

		enum class GoalViewModelField {
			Name = 0,
			Date = 1,
			Amount = 2,
			Accounts = 3
		};
	}
}
