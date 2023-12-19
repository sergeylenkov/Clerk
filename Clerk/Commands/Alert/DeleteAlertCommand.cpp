#include "DeleteAlertCommand.h"

using namespace Clerk::Commands;

DeleteAlertCommand::DeleteAlertCommand(AlertsService& service):
	_service(service),
	_alertId(-1) {
}

void DeleteAlertCommand::SetAlertId(int id) {
	_alertId = id;
}

void DeleteAlertCommand::Execute() {
	auto alert = _service.GetById(_alertId);

	if (alert) {
		_service.Delete(*alert);
	}
}