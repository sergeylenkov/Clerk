#include "EditSchedulerCommand.h"

using namespace Clerk::Commands;

EditSchedulerCommand::EditSchedulerCommand(ICommandsReceiver& receiver) :
	_receiver(receiver),
	_schedulerId(-1) {
}

void EditSchedulerCommand::SetSchedulerId(int id) {
	_schedulerId = id;
}

void EditSchedulerCommand::Execute() {
	_receiver.OpenEditSchedulerDialog(_schedulerId);
}