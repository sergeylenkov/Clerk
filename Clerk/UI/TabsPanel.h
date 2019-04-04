#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Data/Settings.h"
#include "TransactionsListPanel.h"
#include "DashboardPanel.h"
#include "./Reports/ReportExpensesByMonthPanel.h"
#include "./Reports/ReportBalanceByMonthPanel.h"
#include "./Reports/ReportExpensesForPeriodPanel.h"
#include "BudgetsPanel.h"
#include "TrashPanel.h"
#include "SchedulersPanel.h"
#include "GoalsPanel.h"
#include "TagsPanel.h"
#include "TreeMenu.h"

enum class TabsMenuTypes {
	MoveLeft = 1,
	MoveRight = 2,
	Close = 3,
};

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
	std::shared_ptr<Transaction> GetSelectedTransaction();
	std::shared_ptr<Budget> GetSelectedBudget();
	std::shared_ptr<Account> GetSelectedAccount();

	std::function<void(wxString text)> OnUpdateStatus;
	std::function<void()> OnAddTransaction;
	std::function<void(std::shared_ptr<Transaction>)> OnCopyTransaction;
	std::function<void(std::shared_ptr<Transaction>)> OnEditTransaction;
	std::function<void(std::shared_ptr<Transaction>)> OnSplitTransaction;
	std::function<void()> OnAddBudget;
	std::function<void(std::shared_ptr<Budget>)> OnEditBudget;
	std::function<void()> OnAddScheduler;
	std::function<void(std::shared_ptr<Scheduler>)> OnEditScheduler;
	std::function<void()> OnAddGoal;
	std::function<void(std::shared_ptr<Goal>)> OnEditGoal;

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
	void CreateDashboardPanel(int tabIndex);
	void CreateBudgetsPanel(int tabIndex);
	void CreateSchedulersPanel(int tabIndex);
	void CreateGoalsPanel(int tabIndex);
	void CreateReportPanel(int tabIndex, std::shared_ptr<Report> report);
	void CreateTrashPanel(int tabIndex);
	void CreateTagsPanel(int tabIndex);
	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void OnTabMenuClose(wxCommandEvent &event);
	void AddTransaction();
	void CopyTransaction(std::shared_ptr<Transaction> transaction);
	void EditTransaction(std::shared_ptr<Transaction> transaction);
	void SplitTransaction(std::shared_ptr<Transaction> ransaction);
	void UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, std::shared_ptr<Account>);
	void AddBudget();
	void EditBudget(std::shared_ptr<Budget> budget);
	void AddScheduler();
	void EditScheduler(std::shared_ptr<Scheduler> scheduler);
	void AddGoal();
	void EditGoal(std::shared_ptr<Goal> goal);
	void RemoveTab(int index);
};

