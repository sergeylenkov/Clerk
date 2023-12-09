#include "OpenAccountTabCommand.h"

using namespace Clerk::Commands;

OpenAccountTabCommand::OpenAccountTabCommand(ICommandsReceiver* receiver) : _accountId(-1) {
	_receiver = receiver;
}

void OpenAccountTabCommand::SetAccountId(int id) {
	_accountId = id;
}

void OpenAccountTabCommand::Execute() {
	_receiver->OpenAccountTab(_accountId);
}