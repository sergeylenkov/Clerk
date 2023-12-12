#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditAccountCommand : public Command {
		public:
			EditAccountCommand(ICommandsReceiver& _receiver);
			void SetAccountId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _accountId;
		};
	}
}