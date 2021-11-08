#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class TreeContextMenu : public wxMenu {
		public:
			TreeContextMenu(CommandsInvoker& commandsInvoker);
			
		protected:
			CommandsInvoker& _commandsInvoker;
		};
	}
}