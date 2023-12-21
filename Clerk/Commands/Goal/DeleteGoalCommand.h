#pragma once

#include "../Command.h"
#include "../../Data/Services/GoalsService.h"

namespace Clerk {
	namespace Commands {
		class DeleteGoalCommand : public Command {
		public:
			DeleteGoalCommand(GoalsService& service);
			void SetGoalId(int id);
			void Execute() override;

		private:
			GoalsService& _service;
			int _goalId;
		};
	}
}
