#include "DeleteTransactionCommand.h"

using namespace Clerk::Commands;

DeleteTransactionCommand::DeleteTransactionCommand(ICommandsReceiver* receiver) : _transactionId(-1) {
	_receiver = receiver;
}

void DeleteTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void DeleteTransactionCommand::Execute() {
	_receiver->DeleteTransaction(_transactionId);
}