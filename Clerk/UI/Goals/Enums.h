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
	}
}