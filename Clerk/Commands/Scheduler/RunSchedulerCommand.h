#pragma once

#include "../Command.h"
#include "../../Data/Services/SchedulersService.h"

namespace Clerk {
	namespace Commands {
		class RunSchedulerCommand : public Command {
		public:
			RunSchedulerCommand(SchedulersService& service);
			void SetSchedulerId(int id);
			void Execute() override;

		private:
			SchedulersService& _service;
			int _schedulerId;
		};
	}
}
