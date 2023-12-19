#include "PauseSchedulerCommand.h"

using namespace Clerk::Commands;

PauseSchedulerCommand::PauseSchedulerCommand(SchedulersService& service) :
	_service(service),
	_schedulerId(-1) {
}

void PauseSchedulerCommand::SetSchedulerId(int id) {
	_schedulerId = id;
}

void PauseSchedulerCommand::Execute() {
	auto scheduler = _service.GetById(_schedulerId);

	if (scheduler) {
		_service.Pause(*scheduler);
	}
}