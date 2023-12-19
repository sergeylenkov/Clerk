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
	}
}

