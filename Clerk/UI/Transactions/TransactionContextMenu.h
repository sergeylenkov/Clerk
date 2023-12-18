#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TransactionContextMenu : public wxMenu {
		public:
			TransactionContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<TransactionPresentationModel> transaction, std::vector<int> selectedIds);

		private:
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;
			std::shared_ptr<TransactionPresentationModel> _transaction;
			std::vector<int> _selectedIds;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
