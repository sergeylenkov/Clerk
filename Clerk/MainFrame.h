#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <memory>
#include "Data/DataHelper.h"
#include "Defines.h"
#include "Settings.h"
#include "TreeMenuItemData.h"
#include "TransactionFrame.h"
#include "TransactionsListPanel.h"
#include "AccountFrame.h"
#include "HomePanel.h"
#include "ReportPanel.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainFrame();

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	wxImageList *accountsImageList;
	TransactionFrame *transactionFrame;
	TransactionsListPanel *transactionList;	
	AccountFrame *accountFrame;
	HomePanel *homePanel;
	ReportPanel *reportPanel;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
	wxPanel *panel2;
	wxPanel *panel3;
	wxPanel *panel5;
	wxBoxSizer *vbox;
	int selectedAccountId;

	void UpdateAccountsTree();
	void UpdateTransactionList(TreeMenuItemTypes type, Account *account);
	void UpdateStatus();
	void OnQuit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnAddTransaction(wxCommandEvent &event);
	void OnDuplicateTransaction(wxCommandEvent &event);
	void OnSplitTransaction(wxCommandEvent &event);
	void OnAddAccount(wxCommandEvent &event);
	void OnEditAccount(wxCommandEvent &event);
	void OnDeleteAccount(wxCommandEvent &event);
	void OnTreeSpecItemMenu(wxTreeEvent &event);
	void OnTreeItemSelect(wxTreeEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
	void AddTransaction();
	void EditTransaction();
	void SplitTransaction();
	void DeleteTransaction();	
	void OnTransactionClose();
	void AddAccount();
	void EditAccount();
	void DeleteAccount();
	void OnAccountClose();
};