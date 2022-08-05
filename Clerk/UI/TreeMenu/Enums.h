#pragma once

namespace Clerk {
	namespace UI {
		enum class TreeMenuItemType {			
			Dashboard = 0,
			Accounts = 1,
			Account = 2,
			Expenses = 3,
			Receipts = 4,
			Deposits = 5,
			Debts = 6,
			Virtual = 7,
			Reports = 8,
			Report = 9,
			Budgets = 10,			
			Schedulers = 11,
			Goals = 12,
			Tags = 13,
			Alerts = 14,
			Trash = 15,			
			Archive = 16
		};

		enum class ContextMenuType {
			NewTab = 1,
			NewAccount = 2,
			EditAccount = 3,
			DeleteAccount = 4,
			NewTransaction = 5,
			NewBudget = 6,
			NewScheduler = 7,
			RestoreAccount = 8,
			EmptyTrash = 9,
			AddGoal = 10,
			AddAlert = 11
		};
	}
}
