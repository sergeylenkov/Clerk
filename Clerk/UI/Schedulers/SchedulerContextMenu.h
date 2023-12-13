#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class SchedulerContextMenu : public wxMenu {
		public:
			SchedulerContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<SchedulerPresentationModel> scheduler);

		private:
			CommandsInvoker& _commandsInvoker;
			std::shared_ptr<SchedulerPresentationModel> _scheduler;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}
