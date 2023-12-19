#include "NewSchedulerCommand.h"

using namespace Clerk::Commands;

NewSchedulerCommand::NewSchedulerCommand(ICommandsReceiver& receiver):
	_receiver(receiver) {
}

void NewSchedulerCommand::Execute() {
	//
}