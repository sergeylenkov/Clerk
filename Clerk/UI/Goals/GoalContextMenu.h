#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "../../Data/PresentationModels/GoalPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class GoalContextMenu : public wxMenu {
		public:
			GoalContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<GoalPresentationModel> goal);

		private:
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;
			std::shared_ptr<GoalPresentationModel> _goal;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
