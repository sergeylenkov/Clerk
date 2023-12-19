#include "DeleteSchedulerCommand.h"

using namespace Clerk::Commands;

DeleteSchedulerCommand::DeleteSchedulerCommand(SchedulersService& service):
	_service(service),
	_schedulerId(-1) {
}

void DeleteSchedulerCommand::SetSchedulerId(int id) {
	_schedulerId = id;
}

void DeleteSchedulerCommand::Execute() {
	auto scheduler = _service.GetById(_schedulerId);

	if (scheduler) {
		_service.Delete(*scheduler);
	}
}