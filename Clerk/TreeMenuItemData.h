#ifndef TreeMenuItemData_h
#define TreeMenuItemData_h

#include "wx/treectrl.h"
#include "DataHelper.h"

enum TreeMenuItemTypes {
	MenuAccount = 0,
	MenuReport = 1,
	MenuHome = 2,
	MenuBudget = 3,
	MenuExpenses = 4
};

class TreeMenuItemData : public wxTreeItemData {
public:
	TreeMenuItemTypes type;
	shared_ptr<void> object;
};

#endif