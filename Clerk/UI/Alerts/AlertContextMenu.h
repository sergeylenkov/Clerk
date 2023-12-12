#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class AlertContextMenu : public wxMenu {
		public:
			AlertContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<AlertPresentationModel> alert);

		private:
			CommandsInvoker& _commandsInvoker;
			std::shared_ptr<AlertPresentationModel> _alert;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
