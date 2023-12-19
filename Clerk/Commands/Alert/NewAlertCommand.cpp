#include "NewAlertCommand.h"

using namespace Clerk::Commands;

NewAlertCommand::NewAlertCommand(ICommandsReceiver& receiver):
	_receiver(receiver) {
}

void NewAlertCommand::Execute() {
	//
}