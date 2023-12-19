#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class NewSchedulerCommand : public Command {
		public:
			NewSchedulerCommand(ICommandsReceiver& _receiver);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
		};
	}
}