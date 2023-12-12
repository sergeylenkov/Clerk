#pragma once

#include <optional>
#include "Command.h"
#include "ICommandsReceiver.h"
#include "../UI/Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class OpenAccountsTabCommand : public Command {
		public:
			OpenAccountsTabCommand(ICommandsReceiver& _receiver);
			void SetAccountType(AccountType type);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			std::optional<AccountType> _accountType;
		};
	}
}
