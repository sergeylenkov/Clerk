#pragma once

namespace Clerk {
	namespace UI {
		enum class AlertContextMenuTypes {
			Add = 1,
			Edit = 2,
			Delete = 3,
		};

		enum class AlertsListColumns
		{
			Name,
			Message,
			Type,
			Period,
			Condition,
			Importance,
			Amount,
			Last
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
	}
}

