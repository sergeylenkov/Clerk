#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class PreferencesCommand : public Command {
		public:
			PreferencesCommand(ICommandsReceiver* _receiver);

			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
		};
	}
}