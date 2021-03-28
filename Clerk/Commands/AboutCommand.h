#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class AboutCommand : public Command {
		public:
			AboutCommand(ICommandsReceiver* _receiver);

			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
		};
	}
}