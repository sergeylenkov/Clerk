#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class SplitTransactionCommand : public Command {
		public:
			SplitTransactionCommand(ICommandsReceiver& _receiver);
			void SetTransactionId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _transactionId;
		};
	}
}