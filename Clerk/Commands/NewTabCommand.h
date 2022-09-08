#pragma once

#include "Command.h"
#include "ICommandsReceiver.h"
#include "../UI/Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class NewTabCommand : public Command {
		public:
			NewTabCommand(ICommandsReceiver* _receiver);
			void SetType(TabType type);
			void Execute() override;

		private:
			ICommandsReceiver* _receiver;
			TabType _tabType;
		};
	}
}