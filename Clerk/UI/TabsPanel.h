#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include "../Defines.h"
#include "../Data/Settings.h"
#include "../Data/DataContext.h"
#include "Transactions/TransactionsListPanel.h"
#include "Dashboard/DashboardPanel.h"
#include "Reports/ReportExpensesByMonthPanel.h"
#include "Reports/ReportBalanceByMonthPanel.h"
#include "Reports/ReportExpensesForPeriodPanel.h"
#include "Budgets/BudgetsPanel.h"
#include "TrashPanel.h"
#include "Schedulers/SchedulersPanel.h"
#include "Goals/GoalsPanel.h"
#include "Alerts/AlertsPanel.h"
#include "TagsPanel.h"
#include "TreeMenu/Enums.h"

using namespace Clerk::Data;

class TabsPanel : public wxPanel
{
public:
	enum class ContextMenuTypes {
		MoveLeft = 1,
		MoveRight = 2,
		Close = 3,
	};

	TabsPanel(wxWindow *parent, DataContext& context);
	~TabsPanel();

	void CreateTab(TreeMenuItemType type, std::shared_ptr<void> object);
	void AddTab(TreeMenuItemType type, std::shared_ptr<void> object);
	void UpdateCurrentTab(TreeMenuItemType type, std::shared_ptr<void> object);
	void RestoreTabs();
	bool IsTabExists(TreeMenuItemType type, int id = 0);
	void SelectTab(TreeMenuItemType type, int id = 0);
	void Update();
	std::shared_ptr<TransactionViewModel> GetSelectedTransaction();

	std::function<void(wxString text)> OnUpdateStatus;
	std::function<void()> OnAddTransaction;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnCopyTransaction;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnEditTransaction;
	std::function<void(std::shared_ptr<TransactionViewModel>)> OnSplitTransaction;
	std::function<void()> OnAddBudget;
	std::function<void(std::shared_ptr<BudgetViewModel>)> OnEditBudget;
	std::function<void()> OnAddScheduler;
	std::function<void(std::shared_ptr<SchedulerViewModel>)> OnEditScheduler;
	std::function<void()> OnAddGoal;
	std::function<void(std::shared_ptr<GoalViewModel>)> OnEditGoal;
	std::function<void()> OnAddAlert;
	std::function<void(std::shared_ptr<AlertViewModel>)> OnEditAlert;

private:
	Data::DataContext& _context;
	wxNotebook *notebook;
	std::vector<wxPanel *> tabs;
	std::vector<wxBoxSizer *> tabsSizer;
	std::vector<DataPanel *> tabsPanels;
	int contextMenuTab;

	void CreatePanel(int tabIndex, TreeMenuItemType type, std::shared_ptr<void> object);
	void CreateAccountPanel(int tabIndex, std::shared_ptr<AccountViewModel> account);
	void CreateAccountsPanel(int tabIndex, TreeMenuItemType type);
	void CreateDashboardPanel(int tabIndex);
	void CreateBudgetsPanel(int tabIndex);
	void CreateSchedulersPanel(int tabIndex);
	void CreateGoalsPanel(int tabIndex);
	void CreateReportPanel(int tabIndex, std::shared_ptr<ReportViewModel> report);
	void CreateTrashPanel(int tabIndex);
	void CreateTagsPanel(int tabIndex);
	void CreateAlertsPanel(int tabIndex);
	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void OnTabMenuClose(wxCommandEvent &event);
	void AddTransaction();
	void CopyTransaction(std::shared_ptr<TransactionViewModel> transaction);
	void EditTransaction(std::shared_ptr<TransactionViewModel> transaction);
	void SplitTransaction(std::shared_ptr<TransactionViewModel> ransaction);
	void UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemType type, std::shared_ptr<AccountViewModel>);
	void AddBudget();
	void EditBudget(std::shared_ptr<BudgetViewModel> budget);
	void AddScheduler();
	void EditScheduler(std::shared_ptr<SchedulerViewModel> scheduler);
	void AddGoal();
	void EditGoal(std::shared_ptr<GoalViewModel> goal);
	void AddAlert();
	void EditAlert(std::shared_ptr<AlertViewModel> alert);
	void RemoveTab(int index);
};

