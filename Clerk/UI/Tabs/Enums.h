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
			Deposits = 2,
			Receipts = 3,
			Expenses = 4,
			Debts = 5,
			Virtual = 6,
			Reports = 7,
			Budgets = 8,
			Schedulers = 9,
			Goals = 10,
			Tags = 11,
			Alerts = 12,
			Trash = 13
		};
	}
}