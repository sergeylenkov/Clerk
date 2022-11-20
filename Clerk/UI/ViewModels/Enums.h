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
	}
}
