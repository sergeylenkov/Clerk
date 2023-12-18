#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Commands;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TreeContextMenu : public wxMenu {
		public:
			TreeContextMenu(CommandsInvoker& commandsInvoker, Icons& icons);
			
		protected:
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;
		};
	}
}