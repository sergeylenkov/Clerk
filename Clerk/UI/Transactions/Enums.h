#pragma once

namespace Clerk {
	namespace UI {
		enum class TransactionContextMenuType {
			Add = 1,
			Edit = 2,
			Delete = 3,
			Duplicate = 4,
			Split = 5,
			Merge = 6,
			Copy = 7,
		};

		enum class TransactionsListColumns
		{
			Date,
			FromAccount,
			ToAccount,
			Tags,
			Note,
			Amount,
			Last
		};

		enum class TransactionViewModelField {
			FromAccount = 0,
			ToAccount = 1,
			FromAmount = 2,
			ToAmount = 3,
			Date = 4,
			Tags = 5,
			Note = 6,
		};
	}
}