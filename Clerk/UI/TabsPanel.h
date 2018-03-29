#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"
#include "TransactionsListPanel.h"
#include "HomePanel.h"
#include "ReportPanel.h"
#include "BudgetsListPanel.h"

class TabsPanel : public wxPanel
{
public:
	TabsPanel(wxWindow *parent, wxWindowID id);
	~TabsPanel();

	void CreateTab(TreeMenuItemTypes type, shared_ptr<void> object);
	void AddTab(TreeMenuItemTypes type, shared_ptr<void> object);
	void UpdateCurrentTab(TreeMenuItemTypes type, shared_ptr<void> object);
	void RestoreTabs();
	bool IsTabExists(TreeMenuItemTypes type, int id = 0);
	void SelectTab(TreeMenuItemTypes type, int id = 0);
	void Update();
	void UpdateStatus();
	std::shared_ptr<Transaction> GetSelectedTransaction();
	std::shared_ptr<Budget> GetSelectedBudget();
	std::shared_ptr<Account> GetSelectedAccount();

	std::function<void(wxString text)> OnUpdateStatus;
	std::function<void(std::shared_ptr<Transaction>)> OnEditTransaction;
	std::function<void(std::shared_ptr<Transaction>)> OnSplitTransaction;
	std::function<void(std::shared_ptr<Budget>)> OnEditBudget;

private:
	wxNotebook *notebook;
	std::vector<wxPanel *> tabs;
	std::vector<wxBoxSizer *> tabsSizer;
	std::vector<DataPanel *> tabsPanels;
	wxPanel *currentTabPanel;
	int contextMenuTab;

	void CreatePanel(int tabIndex, TreeMenuItemTypes type, shared_ptr<void> object);
	void CreateAccountPanel(int tabIndex, std::shared_ptr<Account> account);
	void CreateAccountsPanel(int tabIndex, TreeMenuItemTypes type);
	void CreateHomePanel(int tabIndex);
	void CreateBudgetsPanel(int tabIndex);
	void CreateReportPanel(int tabIndex, std::shared_ptr<Report> report);
	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void OnTabMenuClose(wxCommandEvent &event);
	void EditTransaction(std::shared_ptr<Transaction> transaction);
	void SplitTransaction(std::shared_ptr<Transaction> ransaction);
	void UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, std::shared_ptr<Account>);
	void RemoveTab(int index);
};

