#pragma once

namespace Clerk {
	namespace UI {
		enum class BudgetContextMenuTypes {
			Add = 1,
			Edit = 2,
			Delete = 3,
		};

		enum class BudgetsListColumns
		{
			Name,
			Period,
			Limit,
			Current,
			Remain,
			Progress,
			Last
		};

		enum class BudgetViewModelField {
			Name = 0,
			Period = 1,
			Date = 2,
			Amount = 3,
			Accounts = 4
		};
	}
}
