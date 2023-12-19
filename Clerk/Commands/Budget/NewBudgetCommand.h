#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class NewBudgetCommand : public Command {
		public:
			NewBudgetCommand(ICommandsReceiver& _receiver);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
		};
	}
}