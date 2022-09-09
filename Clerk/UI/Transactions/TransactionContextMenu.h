#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class TransactionContextMenu : public wxMenu {
		public:
			TransactionContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<TransactionViewModel> transaction);

		private:
			CommandsInvoker& _commandsInvoker;
			std::shared_ptr<TransactionViewModel> _transaction;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
