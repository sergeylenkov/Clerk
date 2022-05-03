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
		class AccountContextMenu : public TreeContextMenu {
		public:
			AccountContextMenu(CommandsInvoker& commandsInvoker, AccountViewModel& account, std::vector<std::shared_ptr<TransactionViewModel>> transactions);

		private:
			AccountViewModel& _account;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}