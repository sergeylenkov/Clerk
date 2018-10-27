#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"

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
	std::function<void()> OnSchedulersSelect;
	std::function<void()> OnTrashSelect;
	std::function<void(TreeMenuItemTypes)> OnAccountsSelect;
	std::function<void()> OnAddAccount;
	std::function<void(std::shared_ptr<Account>)> OnEditAccount;
	std::function<void(std::shared_ptr<Account>)> OnArchiveAccount;
	std::function<void(std::shared_ptr<Account>)> OnRestoreAccount;
	std::function<void(std::shared_ptr<Account>)> OnAddTransaction;
	std::function<void()> OnAddBudget;
	std::function<void()> OnAddScheduler;
	std::function<void(TreeMenuItemTypes type, shared_ptr<void> object)> OnNewTab;
	std::function<void()> OnEmptyTrash;

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	wxImageList *accountsImageList;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
	wxTreeItemId contextMenuItem;

	void CreateImageList();
	std::shared_ptr<Account> GetContextMenuAccount();
	void ExpandItem(wxTreeItemId item);
	void OnTreeSpecItemMenu(wxTreeEvent &event);
	void OnTreeItemSelect(wxTreeEvent &event);
	void OnMenuAddAccount(wxCommandEvent &event);
	void OnMenuEditAccount(wxCommandEvent &event);
	void OnMenuDeleteAccount(wxCommandEvent &event);
	void OnMenuRestoreAccount(wxCommandEvent &event);
	void OnMenuAddTransaction(wxCommandEvent &event);
	void OnMenuAddBudget(wxCommandEvent &event);
	void OnMenuAddScheduler(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void OnMenuEmptyTrash(wxCommandEvent &event);
	void OnTreeItemExpanded(wxTreeEvent &event);
	void OnTreeItemCollapsed(wxTreeEvent &event);	
};

