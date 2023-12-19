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
	}
}
