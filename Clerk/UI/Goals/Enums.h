#pragma once

namespace Clerk {
	namespace UI {
		enum class GoalContextMenuTypes {
			Add = 1,
			Edit = 2,
			Delete = 3,
		};

		enum class GoalsListColumns
		{
			Name,
			DueDate,
			DaysRemain,
			Goal,
			Current,
			Remain,
			Progress,
			Last
		};

		enum class GoalViewModelField {
			Name = 0,
			Date = 1,
			Amount = 2,
			Accounts = 3
		};
	}
}