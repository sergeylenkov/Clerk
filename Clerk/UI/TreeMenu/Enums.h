#pragma once

namespace Clerk {
	namespace UI {
		enum class TreeMenuItemType {
			Account = 0,
			Report = 1,
			Dashboard = 2,
			Credits = 3,
			Expenses = 4,
			Receipts = 5,
			Deposits = 6,
			Accounts = 7,
			Reports = 8,
			Budgets = 9,
			Trash = 10,
			Archive = 11,
			Schedulers = 12,
			Debt = 13,
			Virtual = 14,
			Goals = 15,
			Tags = 16,
			Alerts = 17
		};

		enum class ContextMenuType {
			NewTab = 1,
			AddAccount = 2,
			EditAccount = 3,
			DeleteAccount = 4,
			NewTransaction = 5,
			AddBudget = 6,
			AddScheduler = 7,
			RestoreAccount = 8,
			EmptyTrash = 9,
			AddGoal = 10,
			AddAlert = 11
		};
	}
}
