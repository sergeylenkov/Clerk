#pragma once

#include <wx/wx.h>
#include "Command.h"

namespace Clerk {
	namespace Commands {
		class QuitCommand : public Command {
		public:
			QuitCommand(wxFrame& frame);

			void Execute() override;

		private:
			wxFrame& _frame;
		};
	}
}