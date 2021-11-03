#pragma once

#include <wx/wx.h>
#include "TreeMenu.h"
#include "Enums.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class TransactionContextMenu : public wxMenu {
		public:
			TransactionContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, AccountViewModel& account, std::vector<std::shared_ptr<TransactionViewModel>> transactions, CommandsInvoker& commandsInvoker);

		private:
			CommandsInvoker& _commandsInvoker;
			AccountViewModel& _account;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}