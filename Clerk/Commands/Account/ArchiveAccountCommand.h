#pragma once

#include "../Command.h"
#include "../../Data/Services/AccountsService.h"

namespace Clerk {
	namespace Commands {
		class ArchiveAccountCommand : public Command {
		public:
			ArchiveAccountCommand(AccountsService& service);
			void SetAccountId(int id);
			void Execute() override;

		private:
			AccountsService& _service;
			int _accountId;
		};
	}
}