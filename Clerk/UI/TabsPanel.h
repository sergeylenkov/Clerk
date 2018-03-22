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

	void CreateTab(int type, shared_ptr<void> object);
	void AddTab(int type, shared_ptr<void> object);
	void UpdateCurrentTab(int type, shared_ptr<void> object);
	void RestoreTabs();
	bool IsTabExists(int type, int id = 0);
	void SelectTab(int type, int id = 0);	
	void Update();
	std::shared_ptr<Transaction> GetSelectedTransaction();
	std::shared_ptr<Budget> GetSelectedBudget();

	std::function<void(wxString text)> OnUpdateStatus;
	std::function<void(std::shared_ptr<Transaction> transaction)> OnEditTransaction;
	std::function<void(std::shared_ptr<Transaction> transaction)> OnSplitTransaction;
	std::function<void(std::shared_ptr<Budget> transaction)> OnEditBudget;

private:
	wxNotebook *notebook;
	std::vector<wxPanel *> tabs;
	std::vector<wxBoxSizer *> tabsSizer;
	std::vector<DataPanel *> tabsPanels;
	wxPanel *currentTabPanel;
	
	void CreatePanel(int tabIndex, int type, shared_ptr<void> object);
	void CreateAccountPanel(int tabIndex, std::shared_ptr<Account> account);
	void CreateAccountsPanel(int tabIndex, TreeMenuItemTypes type);
	void CreateHomePanel(int tabIndex);
	void CreateBudgetsPanel(int tabIndex);
	void CreateReportPanel(int tabIndex, std::shared_ptr<Report> report);
	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void OnTabMenuClose(wxCommandEvent &event);
	void EditTransaction();
	void SplitTransaction();
	void UpdateStatus();
	void UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, std::shared_ptr<Account>);
};

