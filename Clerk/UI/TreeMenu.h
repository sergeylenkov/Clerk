#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Data/Settings.h"

enum class TreeMenuTypes {
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
};

enum class TreeMenuItemTypes {
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
	MenuVirtual = 14,
	MenuGoals = 15,
	MenuTags = 16,
};

class TreeMenuItemData : public wxTreeItemData {
public:
	TreeMenuItemTypes type;
	shared_ptr<void> object;
};

class TreeMenu : public wxPanel
{
public:
	TreeMenu(wxWindow *parent, wxWindowID id);
	~TreeMenu();

	void Update();
	void RestoreState();
	std::shared_ptr<Account> GetAccount();
	vector<std::shared_ptr<Account>> GetAccounts();

	std::function<void(std::shared_ptr<Account>)> OnAccountSelect;
	std::function<void(std::shared_ptr<Report>)> OnReportSelect;
	std::function<void()> OnDashboardSelect;
	std::function<void()> OnBudgetsSelect;
	std::function<void()> OnGoalsSelect;
	std::function<void()> OnSchedulersSelect;
	std::function<void()> OnTrashSelect;
	std::function<void()> OnTagsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAccountsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAddAccount;
	std::function<void(std::shared_ptr<Account>)> OnEditAccount;
	std::function<void(std::shared_ptr<Account>)> OnArchiveAccount;
	std::function<void(std::shared_ptr<Account>)> OnRestoreAccount;
	std::function<void(std::shared_ptr<Account>)> OnAddTransaction;
	std::function<void()> OnAddBudget;
	std::function<void()> OnAddScheduler;
	std::function<void()> OnAddGoal;
	std::function<void(TreeMenuItemTypes type, shared_ptr<void> object)> OnNewTab;
	std::function<void()> OnEmptyTrash;

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
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
	void OnMenuAddBudget(wxCommandEvent &event);
	void OnMenuAddScheduler(wxCommandEvent &event);
	void OnMenuAddGoal(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void OnMenuEmptyTrash(wxCommandEvent &event);
	void OnTreeItemExpanded(wxTreeEvent &event);
	void OnTreeItemCollapsed(wxTreeEvent &event);
	void OnBeginDrag(wxTreeEvent &event);
	void OnEndDrag(wxTreeEvent &event);
	void ReorderAccounts(wxTreeItemId &item);
};

