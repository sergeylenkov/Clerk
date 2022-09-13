#include "NewTransactionCommand.h"

using namespace Clerk::Commands;

NewTransactionCommand::NewTransactionCommand(ICommandsReceiver* receiver): _accountId(-1) {
	_receiver = receiver;
}

void NewTransactionCommand::SetAccountId(int id) {
	_accountId = id;
}

void NewTransactionCommand::Execute() {
	_receiver->NewTransaction(_accountId);
}