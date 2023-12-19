#pragma once

#include "../Command.h"
#include "../../Data/Services/BudgetsService.h"

namespace Clerk {
	namespace Commands {
		class DeleteBudgetCommand : public Command {
		public:
			DeleteBudgetCommand(BudgetsService& service);
			void SetBudgetId(int id);
			void Execute() override;

		private:
			BudgetsService& _service;
			int _budgetId;
		};
	}
}
