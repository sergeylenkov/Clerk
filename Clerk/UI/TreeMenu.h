#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Data/Settings.h"

enum class TreeMenuItemTypes {
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

class TreeMenuItemData : public wxTreeItemData {
public:
	TreeMenuItemTypes type;
	shared_ptr<void> object;
};

class TreeMenu : public wxPanel
{
public:
	enum class ContextMenuTypes {
		NewTab = 1,
		AddAccount = 2,
		EditAccount = 3,
		DeleteAccount = 4,
		AddTransaction = 5,
		AddBudget = 6,
		AddScheduler = 7,
		RestoreAccount = 8,
		EmptyTrash = 9,
		AddGoal = 10,
		AddAlert = 11
	};

	TreeMenu(wxWindow *parent, wxWindowID id);

	void Update();
	void RestoreState();
	std::shared_ptr<Account> GetAccount();
	vector<std::shared_ptr<Account>> GetAccounts();
	void UpdateTrashItem();

	std::function<void(std::shared_ptr<Account>)> OnAccountSelect;
	std::function<void(std::shared_ptr<Report>)> OnReportSelect;
	std::function<void()> OnDashboardSelect;
	std::function<void()> OnBudgetsSelect;
	std::function<void()> OnGoalsSelect;
	std::function<void()> OnSchedulersSelect;
	std::function<void()> OnTrashSelect;
	std::function<void()> OnTagsSelect;
	std::function<void()> OnAlertsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAccountsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAddAccount;
	std::function<void(std::shared_ptr<Account>)> OnEditAccount;
	std::function<void(std::shared_ptr<Account>)> OnArchiveAccount;
	std::function<void(std::shared_ptr<Account>)> OnRestoreAccount;
	std::function<void(std::shared_ptr<Account>)> OnAddTransactionForAccount;
	std::function<void(std::shared_ptr<Transaction>)> OnAddTransaction;
	std::function<void()> OnAddBudget;
	std::function<void()> OnAddScheduler;
	std::function<void()> OnAddGoal;
	std::function<void()> OnAddAlert;
	std::function<void(TreeMenuItemTypes type, shared_ptr<void> object)> OnNewTab;
	std::function<void()> OnEmptyTrash;

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
	wxTreeItemId trashItem;
	wxTreeItemId contextMenuItem;
	wxTreeItemId draggedMenuItem;
	int menuImageIndex;

	void CreateImageList();
	std::shared_ptr<Account> GetContextMenuAccount();
	void ExpandItem(wxTreeItemId &item);
	void OnTreeSpecItemMenu(wxTreeEvent &event);
	void OnTreeItemSelect(wxTreeEvent &event);
	void OnMenuAddAccount(wxCommandEvent &event);
	void OnMenuEditAccount(wxCommandEvent &event);
	void OnMenuDeleteAccount(wxCommandEvent &event);
	void OnMenuRestoreAccount(wxCommandEvent &event);
	void OnMenuAddTransaction(wxCommandEvent &event);
	void OnSubMenuAddTransaction(wxCommandEvent &event);
	void OnMenuAddBudget(wxCommandEvent &event);
	void OnMenuAddScheduler(wxCommandEvent &event);
	void OnMenuAddGoal(wxCommandEvent &event);
	void OnMenuAddAlert(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void OnMenuEmptyTrash(wxCommandEvent &event);
	void OnTreeItemExpanded(wxTreeEvent &event);
	void OnTreeItemCollapsed(wxTreeEvent &event);
	void OnBeginDrag(wxTreeEvent &event);
	void OnEndDrag(wxTreeEvent &event);
	void ReorderAccounts(wxTreeItemId &item);
};

