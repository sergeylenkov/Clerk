#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class NewGoalCommand : public Command {
		public:
			NewGoalCommand(ICommandsReceiver& _receiver);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
		};
	}
}