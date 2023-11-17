#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"
#include "../UI/Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class NewAccountsTabCommand : public Command {
		public:
			NewAccountsTabCommand(ICommandsReceiver* _receiver);
			void SetAccountType(AccountType type);
			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
			AccountType _accountType;
		};
	}
}
