#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class OpenReportTabCommand : public Command {
		public:
			OpenReportTabCommand(ICommandsReceiver& _receiver);
			void SetReportId(int id);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			int _reportId;
		};
	}
}