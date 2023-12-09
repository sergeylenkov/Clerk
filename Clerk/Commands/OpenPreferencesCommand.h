#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class OpenPreferencesCommand : public Command {
		public:
			OpenPreferencesCommand(ICommandsReceiver* _receiver);

			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
		};
	}
}