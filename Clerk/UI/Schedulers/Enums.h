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

		enum class SchedulerViewModelField {
			Name = 0,
			FromAccount = 1,
			ToAccount = 2,
			FromAmount = 3,
			ToAmount = 4,
			Tags = 5,
			Type = 6
		};
	}
}
