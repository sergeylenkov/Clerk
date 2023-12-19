#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditBudgetCommand : public Command {
		public:
			EditBudgetCommand(ICommandsReceiver& receiver);
			void SetBudgetId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _budgetId;
		};
	}
}
