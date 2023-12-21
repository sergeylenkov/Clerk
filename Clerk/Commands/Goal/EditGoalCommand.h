#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditGoalCommand : public Command {
		public:
			EditGoalCommand(ICommandsReceiver& receiver);
			void SetGoalId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _goalId;
		};
	}
}
