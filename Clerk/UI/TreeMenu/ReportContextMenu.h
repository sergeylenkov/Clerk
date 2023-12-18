#pragma once

#include <wx/wx.h>
#include "Enums.h"
#include "../PresentationModels/ReportPresentationModel.h"
#include "TreeContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class ReportContextMenu : public TreeContextMenu {
		public:
			ReportContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, ReportPresentationModel& report);

		private:
			ReportPresentationModel& _report;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}