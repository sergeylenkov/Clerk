#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"
#include "../UI/Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class NewAccountTabCommand : public Command {
		public:
			NewAccountTabCommand(ICommandsReceiver* _receiver);
			void SetAccountId(int id);
			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
			int _accountId;
		};
	}
}