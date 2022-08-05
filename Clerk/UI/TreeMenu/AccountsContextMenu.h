#pragma once

#include <wx/wx.h>
#include "Enums.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "TreeContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class AccountsContextMenu : public TreeContextMenu {
		public:
			AccountsContextMenu(CommandsInvoker& commandsInvoker, TreeMenuItemType type);

		private:
			TreeMenuItemType _type;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}