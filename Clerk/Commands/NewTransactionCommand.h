#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class NewTransactionCommand : public Command {
		public:
			NewTransactionCommand(ICommandsReceiver* _receiver);
			void SetTransactionId(int id);
			void SetIsSplit(bool isSplit);
			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
			int _transactionId;
			bool _isSplit;
		};
	}
}