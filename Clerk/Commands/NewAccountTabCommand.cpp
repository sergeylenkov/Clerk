#include "NewAccountTabCommand.h"

using namespace Clerk::Commands;

NewAccountTabCommand::NewAccountTabCommand(ICommandsReceiver* receiver) : _accountId(-1) {
	_receiver = receiver;
}

void NewAccountTabCommand::SetAccountId(int id) {
	_accountId = id;
}

void NewAccountTabCommand::Execute() {
	_receiver->OpenNewAccountTab(_accountId);
}