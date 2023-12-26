#pragma once

namespace Clerk {
	namespace UI {
		enum class SchedulerContextMenuTypes {
			Add = 1,
			Edit = 2,
			Delete = 3,
			Run = 4,
			Pause = 5
		};

		enum class SchedulersListColumns
		{
			Name,
			Type,
			Amount,
			NextDate,
			DaysLeft,
			Status,
			Last
		};
	}
}

#pragma once
