#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../../Data/Settings.h"
#include "../ViewModels/TreeMenuViewModel.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../PresentationModels//AccountPresentationModel.h"
#include "../../Utils/Icons.h"
#include "TreeMenuItemData.h"
#include "AccountContextMenu.h"
#include "AccountsContextMenu.h"
#include "DefaultContextMenu.h"
#include "Enums.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class TreeMenu : public wxPanel
		{
		public:
			TreeMenu(TreeMenuViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons, wxWindow* parent);
			~TreeMenu();

			void RestoreState();

		private:
			TreeMenuViewModel& _viewModel;
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;			
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
			std::shared_ptr<AccountPresentationModel> GetContextMenuAccount();
			void ExpandItem(wxTreeItemId& item);
			void OnTreeSpecItemMenu(wxTreeEvent& event);
			void OnTreeItemSelect(wxTreeEvent& event);			
			void OnTreeItemExpanded(wxTreeEvent& event);
			void OnTreeItemCollapsed(wxTreeEvent& event);
			void OnBeginDrag(wxTreeEvent& event);
			void OnEndDrag(wxTreeEvent& event);
			void ReorderAccounts(wxTreeItemId& item);
			void AddAccountItem(wxTreeItemId& parent, std::shared_ptr<AccountPresentationModel> account);
		};
	}
}
