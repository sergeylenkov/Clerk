#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class QuitCommand : public Command {
		public:
			QuitCommand(ICommandsReceiver* _receiver);

			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
		};
	}
}