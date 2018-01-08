#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <wx/notebook.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"
#include "TreeMenu.h"
#include "TransactionFrame.h"
#include "TransactionsListPanel.h"
#include "AccountFrame.h"
#include "HomePanel.h"
#include "ReportPanel.h"
#include "BudgetFrame.h"
#include "BudgetsListPanel.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainFrame();

private:
	TreeMenu *treeMenu;
	TransactionFrame *transactionFrame;
	TransactionsListPanel *transactionList;	
	AccountFrame *accountFrame;
	BudgetFrame *budgetFrame;
	HomePanel *homePanel;
	ReportPanel *reportPanel;
	BudgetsListPanel *budgetsList;
	wxPanel *transactionsPanelPlaceholder;
	wxPanel *homePanelPlaceholder;
	wxPanel *reportPanelPlaceholder;
	wxPanel *budgetsPanelPlaceholder;
	wxBoxSizer *rightPanelSizer;
	wxNotebook *tabsPanel;

	void UpdateTransactionList(TreeMenuItemTypes type, Account *account);
	void UpdateStatus();
	void OnQuit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnAddTransaction(wxCommandEvent &event);
	void OnDuplicateTransaction(wxCommandEvent &event);
	void OnSplitTransaction(wxCommandEvent &event);
	void HideAllPanels();
	void OnTreeMenuAccountSelect(std::shared_ptr<Account> account);
	void OnTreeMenuReportSelect(std::shared_ptr<Report> report);
	void OnTreeMenuHomeSelect();
	void OnTreeMenuBudgetsSelect();
	void OnAddAccount(wxCommandEvent &event);
	void OnAddBudget(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void AddTransaction();
	void EditTransaction();
	void SplitTransaction();
	void OnTransactionClose();
	void AddAccount();
	void EditAccount(std::shared_ptr<Account> account);
	void DeleteAccount(std::shared_ptr<Account> account);
	void OnAccountClose();
	void AddBudget();
	void EditBudget();
	void OnBudgetClose();
	void UpdateBudgetList();
};