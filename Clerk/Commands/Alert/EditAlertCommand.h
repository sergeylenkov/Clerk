#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

namespace Clerk {
	namespace Commands {
		class EditAlertCommand : public Command {
		public:
			EditAlertCommand(ICommandsReceiver& _receiver);
			void SetAlertId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _alertId;
		};
	}
}
