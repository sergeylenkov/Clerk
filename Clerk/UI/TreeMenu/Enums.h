#pragma once

namespace Clerk {
	namespace UI {
		enum class TreeMenuItemType {			
			Dashboard = 1,
			Accounts = 2,
			Account = 3,
			Expenses = 4,
			Receipts = 5,
			Deposits = 6,
			Debts = 7,
			Virtual = 8,
			Reports = 9,
			Report = 10,
			Budgets = 11,			
			Schedulers = 12,
			Goals = 13,
			Tags = 14,
			Alerts = 15,
			Trash = 16,			
			Archive = 17
		};

		enum class TreeContextMenuType {
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
