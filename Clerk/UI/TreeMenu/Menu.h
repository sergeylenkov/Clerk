#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../../Data/Settings.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/ReportViewModel.h"
#include "../../Utils/Icons.h"
#include "MenuItemData.h"
#include "ContextMenu.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TreeMenu : public wxPanel
		{
		public:
			TreeMenu(wxWindow* parent, Icons* icons);

			void RestoreState();
			void SetReceipts(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetDeposits(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetExpenses(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetDebts(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetVirtuals(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetArchive(std::vector<std::shared_ptr<AccountViewModel>> accounts);
			void SetReports(std::vector<std::shared_ptr<ReportViewModel>> reports);
			void SetIsTrashEmpty(bool isEmpty);

			std::function<std::vector<std::shared_ptr<TransactionViewModel>>(const AccountViewModel& account)> OnContextMenu;

			/*std::function<void(std::shared_ptr<AccountModel>)> OnAccountSelect;
			std::function<void(std::shared_ptr<Report>)> OnReportSelect;
			std::function<void()> OnDashboardSelect;
			std::function<void()> OnBudgetsSelect;
			std::function<void()> OnGoalsSelect;
			std::function<void()> OnSchedulersSelect;
			std::function<void()> OnTrashSelect;
			std::function<void()> OnTagsSelect;
			std::function<void()> OnAlertsSelect;
			std::function<void(TreeMenuItemType type)> OnAccountsSelect;
			std::function<void(TreeMenuItemType type)> OnAddAccount;
			std::function<void(std::shared_ptr<AccountModel>)> OnEditAccount;
			std::function<void(std::shared_ptr<AccountModel>)> OnArchiveAccount;
			std::function<void(std::shared_ptr<AccountModel>)> OnRestoreAccount;
			std::function<void(std::shared_ptr<AccountModel>)> OnAddTransactionForAccount;
			std::function<void(std::shared_ptr<Transaction>)> OnAddTransaction;
			std::function<void()> OnAddBudget;
			std::function<void()> OnAddScheduler;
			std::function<void()> OnAddGoal;
			std::function<void()> OnAddAlert;
			std::function<void(TreeMenuItemType type, std::shared_ptr<void> object)> OnNewTab;
			std::function<void()> OnEmptyTrash;*/

		private:
			Icons* _icons;
			wxTreeCtrl* _treeMenu;
			wxTreeItemId _receiptsItem;
			wxTreeItemId _depositsItem;
			wxTreeItemId _expensesItem;
			wxTreeItemId _debtsItem;
			wxTreeItemId _virtualItem;
			wxTreeItemId _archiveItem;
			wxTreeItemId _reportsItem;
			wxTreeItemId _trashItem;
			wxTreeItemId _contextMenuItem;
			wxTreeItemId _draggedMenuItem;

			void CreateMenu();
			std::shared_ptr<AccountModel> GetContextMenuAccount();
			void ExpandItem(wxTreeItemId& item);
			void OnTreeSpecItemMenu(wxTreeEvent& event);
			void OnTreeItemSelect(wxTreeEvent& event);			
			void OnTreeItemExpanded(wxTreeEvent& event);
			void OnTreeItemCollapsed(wxTreeEvent& event);
			void OnBeginDrag(wxTreeEvent& event);
			void OnEndDrag(wxTreeEvent& event);
			void ReorderAccounts(wxTreeItemId& item);
			void AddAccountItem(wxTreeItemId& parent, std::shared_ptr<AccountViewModel> account);
		};
	}
}
