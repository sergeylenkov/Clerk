#include "DeleteTransactionCommand.h"

using namespace Clerk::Commands;

DeleteTransactionCommand::DeleteTransactionCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_transactionId(-1) {
}

void DeleteTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void DeleteTransactionCommand::Execute() {
	_receiver.DeleteTransaction(_transactionId);
}