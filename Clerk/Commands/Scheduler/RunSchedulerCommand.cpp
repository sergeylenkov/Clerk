#include "RunSchedulerCommand.h"

using namespace Clerk::Commands;

RunSchedulerCommand::RunSchedulerCommand(SchedulersService& service) :
	_service(service),
	_schedulerId(-1) {
}

void RunSchedulerCommand::SetSchedulerId(int id) {
	_schedulerId = id;
}

void RunSchedulerCommand::Execute() {
	auto scheduler = _service.GetById(_schedulerId);

	if (scheduler) {
		_service.Run(*scheduler);
	}
}