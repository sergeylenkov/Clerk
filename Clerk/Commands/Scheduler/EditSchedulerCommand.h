#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditSchedulerCommand : public Command {
		public:
			EditSchedulerCommand(ICommandsReceiver& _receiver);
			void SetSchedulerId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _schedulerId;
		};
	}
}

