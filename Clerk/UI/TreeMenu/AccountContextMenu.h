#pragma once

#include <wx/wx.h>
#include "Enums.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "TreeContextMenu.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class AccountContextMenu : public TreeContextMenu {
		public:
			AccountContextMenu(CommandsInvoker& commandsInvoker, AccountPresentationModel& account, std::vector<std::shared_ptr<TransactionPresentationModel>> transactions, Icons& icons);

		private:
			AccountPresentationModel& _account;
			Icons& _icons;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}