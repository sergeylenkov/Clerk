#include "NewTransactionCommand.h"

using namespace Clerk::Commands;

NewTransactionCommand::NewTransactionCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_accountId(-1) {
}

void NewTransactionCommand::SetAccountId(int id) {
	_accountId = id;
}

void NewTransactionCommand::Execute() {
	_receiver.NewTransaction(_accountId);
}