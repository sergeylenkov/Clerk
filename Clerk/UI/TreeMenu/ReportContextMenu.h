#pragma once

#include <wx/wx.h>
#include "Enums.h"
#include "../PresentationModels/ReportPresentationModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "TreeContextMenu.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class ReportContextMenu : public TreeContextMenu {
		public:
			ReportContextMenu(CommandsInvoker& commandsInvoker, ReportPresentationModel& report);

		private:
			ReportPresentationModel& _report;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}