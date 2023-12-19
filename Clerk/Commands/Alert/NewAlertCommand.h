#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class NewAlertCommand : public Command {
		public:
			NewAlertCommand(ICommandsReceiver& _receiver);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
		};
	}
}