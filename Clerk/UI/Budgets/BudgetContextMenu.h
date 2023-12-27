#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "../../Data/PresentationModels/BudgetPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class BudgetContextMenu : public wxMenu {
		public:
			BudgetContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<BudgetPresentationModel> budget);

		private:
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;
			std::shared_ptr<BudgetPresentationModel> _budget;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
