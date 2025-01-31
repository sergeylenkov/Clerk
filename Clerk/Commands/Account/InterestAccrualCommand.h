#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class InterestAccrualCommand : public Command {
		public:
			InterestAccrualCommand(ICommandsReceiver& receiver);
			void SetAccountId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _accountId;
		};
	}
}
