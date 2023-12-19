#pragma once

#include "../Command.h"
#include "../../Data/Services/TransactionsService.h"

namespace Clerk {
	namespace Commands {
		class DuplicateTransactionCommand : public Command {
		public:
			DuplicateTransactionCommand(TransactionsService& service);
			void SetTransactionId(int id);
			void Execute() override;

		private:
			TransactionsService& _service;
			int _transactionId;
		};
	}
}