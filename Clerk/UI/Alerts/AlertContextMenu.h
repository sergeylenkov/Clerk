#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "../../Data/PresentationModels/AlertPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class AlertContextMenu : public wxMenu {
		public:
			AlertContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<AlertPresentationModel> alert);

		private:
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;
			std::shared_ptr<AlertPresentationModel> _alert;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
