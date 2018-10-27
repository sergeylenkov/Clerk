#pragma once

#include "wx/treectrl.h"
#include "Data/DataHelper.h"

enum TreeMenuItemTypes {
	MenuAccount = 0,
	MenuReport = 1,
	MenuDashboard = 2,
	MenuCredits = 3,
	MenuExpenses = 4,
	MenuReceipts = 5,
	MenuDeposits = 6,
	MenuAccounts = 7,
	MenuReports = 8,
	MenuBudgets = 9,
	MenuTrash = 10,
	MenuArchive = 11,
	MenuSchedulers = 12,
	MenuDebt = 13,
};

class TreeMenuItemData : public wxTreeItemData {
public:
	TreeMenuItemTypes type;
	shared_ptr<void> object;
};