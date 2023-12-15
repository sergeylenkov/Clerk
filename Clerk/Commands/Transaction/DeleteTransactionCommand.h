#pragma once

#include "../Command.h"
#include "../../Data/Services/TransactionsService.h"

namespace Clerk {
	namespace Commands {
		class DeleteTransactionCommand : public Command {
		public:
			DeleteTransactionCommand(TransactionsService& service);
			void SetTransactionId(int id);
			void Execute() override;

		private:
			TransactionsService& _service;
			int _transactionId;
		};
	}
}