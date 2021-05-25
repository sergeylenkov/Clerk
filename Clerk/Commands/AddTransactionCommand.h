#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class AddTransactionCommand : public Command {
		public:
			AddTransactionCommand(ICommandsReceiver* _receiver);

			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
		};
	}
}