#pragma once

#include "../Command.h"
#include "../ICommandsReceiver.h"
#include "../../UI/Tabs/Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class OpenTabCommand : public Command {
		public:
			OpenTabCommand(ICommandsReceiver& _receiver);
			void SetType(TabType type);
			void Execute() override;

		private:
			ICommandsReceiver& _receiver;
			TabType _tabType;
		};
	}
}