#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"

class TreeMenu : public wxPanel
{
public:
	TreeMenu(wxWindow *parent, wxWindowID id);
	~TreeMenu();

	void Update();
	void RestoreState();
	std::shared_ptr<Account> GetAccount();
	vector<std::shared_ptr<Account>> GetAccounts();

	std::function<void(std::shared_ptr<Account> account)> OnAccountSelect;
	std::function<void(std::shared_ptr<Report> report)> OnReportSelect;
	std::function<void()> OnHomeSelect;
	std::function<void()> OnBudgetsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAccountsSelect;
	std::function<void()> OnAddAccount;
	std::function<void(std::shared_ptr<Account> account)> OnEditAccount;
	std::function<void(std::shared_ptr<Account> account)> OnDeleteAccount;
	std::function<void(std::shared_ptr<Account> account)> OnAddTransaction;
	std::function<void(int type, shared_ptr<void> object)> OnNewTab;

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	wxImageList *accountsImageList;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
	wxTreeItemId contextMenuItem;

	void CreateImageList();
	void ExpandItem(wxTreeItemId item);
	void OnTreeSpecItemMenu(wxTreeEvent &event);
	void OnTreeItemSelect(wxTreeEvent &event);
	void OnMenuAddAccount(wxCommandEvent &event);
	void OnMenuEditAccount(wxCommandEvent &event);
	void OnMenuDeleteAccount(wxCommandEvent &event);
	void OnMenuAddTransaction(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void OnTreeItemExpanded(wxTreeEvent &event);
	void OnTreeItemCollapsed(wxTreeEvent &event);	
};

