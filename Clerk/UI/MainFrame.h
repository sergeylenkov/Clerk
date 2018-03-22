#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"
#include "TreeMenu.h"
#include "TransactionFrame.h"
#include "AccountFrame.h"
#include "BudgetFrame.h"
#include "TabsPanel.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainFrame();

private:
	TreeMenu *treeMenu;
	TransactionFrame *transactionFrame;
	AccountFrame *accountFrame;
	BudgetFrame *budgetFrame;
	wxPanel *transactionsPanelPlaceholder;
	wxPanel *homePanelPlaceholder;
	wxPanel *reportPanelPlaceholder;
	wxPanel *budgetsPanelPlaceholder;
	wxBoxSizer *rightPanelSizer;
	TabsPanel *tabsPanel;
	
	void UpdateStatus(wxString text);
	void OnQuit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnAddTransaction(wxCommandEvent &event);
	void OnDuplicateTransaction(wxCommandEvent &event);
	void OnSplitTransaction(wxCommandEvent &event);
	void OnTreeMenuAccountSelect(std::shared_ptr<Account> account);
	void OnTreeMenuReportSelect(std::shared_ptr<Report> report);
	void OnTreeMenuHomeSelect();
	void OnTreeMenuBudgetsSelect();
	void OnTreeMenuAccountsSelect(TreeMenuItemTypes type);
	void OnAddAccount(wxCommandEvent &event);
	void OnAddBudget(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
	void AddTransaction(std::shared_ptr<Account> account);
	void EditTransaction(std::shared_ptr<Transaction> transaction);
	void SplitTransaction(std::shared_ptr<Transaction> transaction);
	void OnTransactionClose();
	void AddAccount();
	void EditAccount(std::shared_ptr<Account> account);
	void DeleteAccount(std::shared_ptr<Account> account);
	void OnAccountClose();
	void AddBudget();
	void EditBudget(std::shared_ptr<Budget> budget);
	void OnBudgetClose();	
	void AddTab(int type, shared_ptr<void> object);	
};