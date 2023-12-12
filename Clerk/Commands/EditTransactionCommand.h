#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditTransactionCommand : public Command {
		public:
			EditTransactionCommand(ICommandsReceiver& _receiver);
			void SetTransactionId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _transactionId;
		};
	}
}