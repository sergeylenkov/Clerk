#pragma once

namespace Clerk {
	namespace UI {
		enum class TabsContextMenuType {
			Left = 1,
			Right = 2,
			Close = 3
		};

		enum class TabType {
			Dashboard = 0,
			Transactions = 1,
			Reports = 3,
			Budgets = 4,
			Schedulers = 5,
			Goals = 6,
			Tags = 7,
			Alerts = 8,
			Trash = 9
		};
	}
}