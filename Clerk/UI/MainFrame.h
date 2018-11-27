#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <memory>
#include <thread>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"
#include "TreeMenu.h"
#include "TransactionDialog.h"
#include "AccountDialog.h"
#include "BudgetDialog.h"
#include "SchedulerDialog.h"
#include "SchedulersConfirmDialog.h"
#include "TabsPanel.h"
#include "GoalDialog.h"

enum class MainMenuTypes {
	About = 1,
	AddTransaction = 2,
	AddAccount = 3,
	AddBudget = 4,
	AddScheduler = 5,
	Exit = 6,
	AddGoal = 7,
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainFrame();

private:
	TreeMenu *treeMenu;
	TransactionDialog *transactionFrame;
	AccountDialog *accountFrame;
	wxBoxSizer *rightPanelSizer;
	TabsPanel *tabsPanel;
	
	void CreateMainMenu();
	void UpdateStatus(wxString text);
	void OnQuit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnAddTransaction(wxCommandEvent &event);
	void OnDuplicateTransaction(wxCommandEvent &event);
	void OnSplitTransaction(wxCommandEvent &event);
	void OnTreeMenuAccountSelect(std::shared_ptr<Account> account);
	void OnTreeMenuReportSelect(std::shared_ptr<Report> report);
	void OnTreeMenuDashboardSelect();
	void OnTreeMenuBudgetsSelect();
	void OnTreeMenuSchedulersSelect();
	void OnTreeMenuTrashSelect();
	void OnTreeMenuAccountsSelect(TreeMenuItemTypes type);
	void OnTreeMenuAddTransaction(std::shared_ptr<Account> account);
	void OnAddAccount(wxCommandEvent &event);
	void OnAddBudget(wxCommandEvent &event);
	void OnAddScheduler(wxCommandEvent &event);
	void OnAddGoal(wxCommandEvent &event);
	void AddTransaction(Account *account);
	void AddTransactionFromContextMenu();
	void CopyTransaction(std::shared_ptr<Transaction> transaction);
	void EditTransaction(std::shared_ptr<Transaction> transaction);
	void SplitTransaction(std::shared_ptr<Transaction> transaction);
	void OnTransactionClose();
	void AddAccount();
	void EditAccount(std::shared_ptr<Account> account);
	void DeleteAccount(std::shared_ptr<Account> account);
	void RestoreAccount(std::shared_ptr<Account> account);
	void OnAccountClose();
	void AddBudget();
	void EditBudget(std::shared_ptr<Budget> budget);
	void OnBudgetClose();	
	void AddScheduler();
	void EditScheduler(std::shared_ptr<Scheduler> scheduler);
	void OnSchedulerClose();
	void AddGoal();
	void EditGoal(std::shared_ptr<Goal> goal);
	void OnGoalClose();
	void AddTab(TreeMenuItemTypes type, shared_ptr<void> object);
	void CheckSchedulers();
	void OnSchedulersConfirmClose();
	void OnEmptyTrash();
	void OnAddMenuTransaction(wxCommandEvent &event);
};