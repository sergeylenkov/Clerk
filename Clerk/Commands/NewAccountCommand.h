#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"
#include "../Data/Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Commands {
		class NewAccountCommand : public Command {
		public:
			NewAccountCommand(ICommandsReceiver& _receiver);
			void SetAccountType(AccountType type);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			AccountType _accountType;
		};
	}
}