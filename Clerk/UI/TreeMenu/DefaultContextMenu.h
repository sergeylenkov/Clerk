#pragma once

#include <wx/wx.h>
#include "Enums.h"
#include "TreeContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class DefaultContextMenu : public TreeContextMenu {
		public:
			DefaultContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, TreeMenuItemType type);

		private:
			TreeMenuItemType _type;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}