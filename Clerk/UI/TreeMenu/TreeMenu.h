#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../../Data/Settings.h"
#include "../../Data/ViewModels/TreeMenuViewModel.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/ReportViewModel.h"
#include "../../Utils/Icons.h"
#include "TreeMenuItemData.h"
#include "TreeContextMenu.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TreeMenu : public wxPanel
		{
		public:
			TreeMenu(wxWindow* parent, Icons* icons);
			~TreeMenu();

			void RestoreState();
			void SetViewModel(TreeMenuViewModel* viewModel);

		private:
			TreeMenuViewModel* _viewModel;
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
			void Update();
			void SetIsTrashEmpty(bool isEmpty);
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
