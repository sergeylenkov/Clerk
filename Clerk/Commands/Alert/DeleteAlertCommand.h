#pragma once

#include "../Command.h"
#include "../../Data/Services/AlertsService.h"

namespace Clerk {
	namespace Commands {
		class DeleteAlertCommand : public Command {
		public:
			DeleteAlertCommand(AlertsService& service);
			void SetAlertId(int id);
			void Execute() override;

		private:
			AlertsService& _service;
			int _alertId;
		};
	}
}
