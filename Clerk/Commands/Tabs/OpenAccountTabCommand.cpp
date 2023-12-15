#include "OpenAccountTabCommand.h"

using namespace Clerk::Commands;

OpenAccountTabCommand::OpenAccountTabCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_accountId(-1) {
}

void OpenAccountTabCommand::SetAccountId(int id) {
	_accountId = id;
}

void OpenAccountTabCommand::Execute() {
	_receiver.OpenAccountTab(_accountId);
}